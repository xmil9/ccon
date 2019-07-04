#include "common.h"
#include "ConsoleWnd.h"
#include "ConsoleContent.h"
#include "Cvfc/Color.h"
#include "Win32DeviceContext.h"
#include "Win32Geometry.h"
#include <tchar.h>

using namespace cvfc;
using namespace std;
using namespace win32;


namespace
{

///////////////////

constexpr UINT TextFormatFlags = DT_LEFT | DT_TOP | DT_NOPREFIX | DT_EXTERNALLEADING;
constexpr UINT CursorBlinkRateMs = 500;
constexpr long CursorWidth = 1;


///////////////////

// Returns the vertical DPI of the screen.
int DpiY()
{
   const HDC screenDc = GetDC(NULL);
   return GetDeviceCaps(screenDc, LOGPIXELSY);
}


// Returns the horizontal DPI of the screen.
int DpiX()
{
   const HDC screenDc = GetDC(NULL);
   return GetDeviceCaps(screenDc, LOGPIXELSX);
}


// Converts a point size (1/72 of an inch) to a raw pixel size.
int PointsToPixels(int points)
{
   return MulDiv(points, DpiY(), 72);
}


template <typename Int> Int TruncatedDiv(Int divident, Int divisor)
{
   static_assert(std::is_integral_v<Int> && !std::is_same_v<Int, bool>,
                 "Integral type expected.");
   return (divident / divisor) * divisor;
}


template <typename Int> Int TruncatedDivRest(Int divident, Int divisor)
{
   static_assert(std::is_integral_v<Int> && !std::is_same_v<Int, bool>,
                 "Integral type expected.");
   return (divident - TruncatedDiv(divident, divisor));
}


COLORREF ColorrefFromRgb(const Rgb& rgb)
{
   return RGB(rgb.r, rgb.g, rgb.b);
}


GdiObj<HFONT> MakeConsoleFont(int heightInPoints)
{
   LOGFONT lf;
   strcpy_s(lf.lfFaceName, _countof(lf.lfFaceName), "Consolas");
   lf.lfHeight = -PointsToPixels(heightInPoints);
   lf.lfWeight = FW_EXTRALIGHT;
   lf.lfStrikeOut = false;
   lf.lfUnderline = false;
   lf.lfItalic = false;
   lf.lfQuality = DEFAULT_QUALITY;
   lf.lfCharSet = DEFAULT_CHARSET;
   lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
   lf.lfEscapement = 0;
   lf.lfOrientation = 0;
   lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
   // FF_MODERN indicates that we want a monospace (fixed char width) font.
   lf.lfPitchAndFamily = DEFAULT_PITCH | FF_MODERN;
   lf.lfWidth = 0;

   return GdiObj<HFONT>(CreateFontIndirect(&lf));
}


///////////////////

// Applies some given and some predefined attributes to a given DC for
// drawing a console.
class ConsoleDCAttributes
{
 public:
   ConsoleDCAttributes(HDC hdc, HFONT font, COLORREF textColor);
   ~ConsoleDCAttributes();
   ConsoleDCAttributes(const ConsoleDCAttributes&) = delete;
   ConsoleDCAttributes(ConsoleDCAttributes&&) = default;
   ConsoleDCAttributes& operator=(const ConsoleDCAttributes&) = delete;
   ConsoleDCAttributes& operator=(ConsoleDCAttributes&&) = default;

 private:
   HDC m_hdc = NULL;
   HFONT m_prevFont = NULL;
   COLORREF m_prevTextColor = 0;
   int m_prevBkMode = 0;
};


ConsoleDCAttributes::ConsoleDCAttributes(HDC hdc, HFONT font, COLORREF textColor)
: m_hdc{hdc}
{
   if (m_hdc)
   {
      m_prevFont = (HFONT)SelectObject(m_hdc, font);
      m_prevTextColor = SetTextColor(m_hdc, textColor);
      m_prevBkMode = SetBkMode(m_hdc, TRANSPARENT);
   }
}

ConsoleDCAttributes::~ConsoleDCAttributes()
{
   if (m_hdc)
   {
      SetBkMode(m_hdc, m_prevBkMode);
      SetTextColor(m_hdc, m_prevTextColor);
      SelectObject(m_hdc, m_prevFont);
   }
}

} // namespace


///////////////////

ConsoleWnd::ConsoleWnd(const ConsoleUIUserPrefs& prefs, ConsoleContent& content)
: m_userPrefs{prefs}, m_content{content}, m_layout{content}
{
}


void ConsoleWnd::SetBackgroundColor(const Rgb& color)
{
   m_backgroundBrush =
      std::move(GdiObj<HBRUSH>(CreateSolidBrush(RGB(color.r, color.g, color.b))));
   inval(true);

   m_userPrefs.SetBackgroundColor(color);
}


void ConsoleWnd::SetOutputTextColor(const Rgb& color)
{
   m_textOutputColor = RGB(color.r, color.g, color.b);
   inval(true);

   m_userPrefs.SetTextOutputColor(color);
}


void ConsoleWnd::SetInputTextColor(const Rgb& color)
{
   m_textInputColor = RGB(color.r, color.g, color.b);
   inval(true);

   m_userPrefs.SetTextInputColor(color);
}


void ConsoleWnd::ResetColors()
{
   SetBackgroundColor(ConsoleUIDefaultPrefs::BackgroundDefaultColor());
   SetOutputTextColor(ConsoleUIDefaultPrefs::TextOutputDefaultColor());
   SetInputTextColor(ConsoleUIDefaultPrefs::TextInputDefaultColor());
}


void ConsoleWnd::SetFontSize(int sizeInPoints)
{
   m_font = MakeConsoleFont(sizeInPoints);
   
   // Changing the font size requires the entire layout to be recalculated.
   m_isLayoutInited = false;
   inval(true);

   m_userPrefs.SetFontSize(sizeInPoints);
}


const TCHAR* ConsoleWnd::windowClassName() const
{
   return _T("ConsoleWndClass");
}


bool ConsoleWnd::registerWindowClass() const
{
   if (isWindowClassRegistered())
      return true;

   const HINSTANCE appInst = appInstance();

   WNDCLASSEX classInfo;
   std::memset(&classInfo, 0, sizeof(WNDCLASSEX));
   classInfo.cbSize = sizeof(WNDCLASSEX);
   classInfo.lpszClassName = windowClassName();
   classInfo.lpfnWndProc = windowProc;
   classInfo.hInstance = appInst;
   classInfo.style = 0;
   classInfo.cbClsExtra = 0;
   classInfo.cbWndExtra = 0;
   classInfo.hIcon = NULL;
   classInfo.hIconSm = 0;
   classInfo.hCursor = LoadCursor(NULL, IDC_ARROW);
   // The system deletes the background brush when the class is unregistered.
   classInfo.hbrBackground = NULL; // CreateSolidBrush(RGB(0, 0, 0));
   classInfo.lpszMenuName = 0;

   const ATOM token = RegisterClassEx(&classInfo);
   assert(token != 0);
   return (token != 0);
}


Window::CreationResult ConsoleWnd::onCreate(const CREATESTRUCT* createInfo)
{
   Window::onCreate(createInfo);

   if (!SetupColors())
      return CreationResult::Abort;
   if (!SetupFont())
      return CreationResult::Abort;
   if (!SetupBackgroundBrush())
      return CreationResult::Abort;

   return CreationResult::Handled;
}


bool ConsoleWnd::onDestroy()
{
   CleanupInputCursor();

   Window::onDestroy();
   return true;
}


bool ConsoleWnd::onPaint()
{
   // Only draw if there is an area that needs updating.
   if (haveInvalBounds())
   {
      win32::PaintDC pdc{hwnd()};
      if (pdc)
      {
         ConsoleDCAttributes dcAttribs{pdc, m_font, m_textOutputColor};

         if (pdc.paintInfo().fErase)
            DrawBackground(pdc, pdc.paintInfo().rcPaint);

         DrawContent(pdc.hdc(), pdc.paintInfo().rcPaint);
         DrawInputCursor(pdc.hdc());
      }
   }

   return true;
}


bool ConsoleWnd::onEraseBkgnd(HDC hdc)
{
   // We draw the background in the paint handler.
   return false;
}


bool ConsoleWnd::onSize(long width, long height, UINT resizeFlag)
{
   if (!m_isLayoutInited)
      return false;

   m_layout.CalcContentMetrics(IntRect{clientBounds()});
   UpdateScrollbar();
   UpdateInputCursor();
   inval(true);

   m_userPrefs.SetConsoleWidth(width);
   m_userPrefs.SetConsoleHeight(height);
   return true;
}


bool ConsoleWnd::onKeyDown(UINT virtKeyCode, UINT repeatCount, BYTE scanCode,
                           bool isExtendedKey, bool wasPreviouslyDown)
{
   return HandleEditKey(virtKeyCode);
}


bool ConsoleWnd::onChar(TCHAR ch, UINT repeatCount, BYTE scanCode, bool isExtendedKey,
                        bool wasPreviouslyDown, bool isAltDown, bool isReleased)
{
   return HandleInputKey(ch);
}


bool ConsoleWnd::onSetFocus(HWND unfocusedWnd)
{
   m_inputCursor.Start();
   return true;
}


bool ConsoleWnd::onKillFocus(HWND focusedWnd)
{
   m_inputCursor.Stop();
   return true;
}


bool ConsoleWnd::onVScroll(UINT scrollAction, UINT thumbPos, HWND /*scrollCtrl*/)
{
   ScrollVertical(scrollAction, thumbPos);
   return true;
}


bool ConsoleWnd::onMouseWheel(int delta, UINT keyState, win32::Point mousePos)
{
   // The passed delta value could be smaller than the wheel delta (the threshold for
   // wheel scrolling. If delta is smaller then we have to accumalate it until it reaches
   // the threshold.
   static int accumulatedDelta = 0;

   constexpr UINT kDefaultNumLines = 3;
   UINT numLinesPerWheelDelta = kDefaultNumLines;
   if (!::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &numLinesPerWheelDelta, 0))
      numLinesPerWheelDelta = kDefaultNumLines;

   accumulatedDelta += delta;
   const bool scrollUp = accumulatedDelta > 0;

   // The passed delta could also be (multiple times) larger than the wheel delta.
   // Process large deltas iteratively as individual scrolls.
   while (std::abs(accumulatedDelta) >= WHEEL_DELTA)
   {
      ScrollVerticalBy(scrollUp ? -static_cast<int>(numLinesPerWheelDelta)
                                : numLinesPerWheelDelta);
      accumulatedDelta += scrollUp ? -WHEEL_DELTA : WHEEL_DELTA;
   }

   return true;
}


bool ConsoleWnd::SetupColors()
{
   bool haveColor = false;
   Rgb color;

   m_textOutputColor = ColorrefFromRgb(ConsoleUIDefaultPrefs::TextOutputDefaultColor());
   tie(haveColor, color) = m_userPrefs.TextOutputColor();
   if (haveColor)
      m_textOutputColor = RGB(color.r, color.g, color.b);

   m_textInputColor = ColorrefFromRgb(ConsoleUIDefaultPrefs::TextInputDefaultColor());
   tie(haveColor, color) = m_userPrefs.TextInputColor();
   if (haveColor)
      m_textInputColor = RGB(color.r, color.g, color.b);

   return true;
}


bool ConsoleWnd::SetupFont()
{
   bool haveFontSize = false;
   int fontSize = 0;
   tie(haveFontSize, fontSize) = m_userPrefs.FontSize();
   if (!haveFontSize)
      fontSize = ConsoleUIDefaultPrefs::FontSize();
   
   SetFontSize(fontSize);
   return !!m_font;
}


bool ConsoleWnd::SetupBackgroundBrush()
{
   bool haveColor = false;
   Rgb color;
   tie(haveColor, color) = m_userPrefs.BackgroundColor();
   if (!haveColor)
      color = ConsoleUIDefaultPrefs::BackgroundDefaultColor();

   SetBackgroundColor(color);
   return !!m_backgroundBrush;
}


void ConsoleWnd::UpdateContentLayout(HDC hdc)
{
   m_layout.CalcGeneralTextMetrics(hdc);
   m_layout.CalcContentMetrics(IntRect{clientBounds()});
   m_isLayoutInited = true;

   // Changes to the layout affect other parts of the UI.
   UpdateScrollbar();
   UpdateInputCursor();
}


void ConsoleWnd::UpdateInputLine(HDC hdc)
{
   const std::size_t prevNumLines = m_layout.CountPhysicalLines();
   m_layout.CalcInputLineMetrics(IntRect{clientBounds()});

   const std::size_t numLines = m_layout.CountPhysicalLines();
   if (numLines != prevNumLines)
      UpdateScrollbar();
   ScrollIntoView();
   UpdateInputCursor();
}


void ConsoleWnd::InvalInputLine()
{
   const win32::Rect bounds = m_layout.LogicalLineBounds(GetContent().CountLines() - 1);
   inval(bounds, true);
}


void ConsoleWnd::RefreshInputLine()
{
   SharedDC consoleDC{::GetDC(hwnd()), hwnd()};
   if (consoleDC)
   {
      ConsoleDCAttributes dcAttribs{consoleDC, m_font, m_textOutputColor};

      UpdateInputLine(consoleDC);
      InvalInputLine();
   }
}


void ConsoleWnd::UpdateInputCursor()
{
   m_inputCursor.Stop();

   m_inputCursor = std::move(ConsoleInputCursor{hwnd(), m_textInputColor});
   m_inputCursor.SetBlinkRate(CursorBlinkRateMs);
   m_inputCursor.SetWidth(CursorWidth);
   m_inputCursor.SetHeight(m_layout.TextHeight());
   m_inputCursor.SetTopLeftPosition(win32::Point(m_layout.InputCursorPixelPosition()));
   m_inputCursor.Start();
}


void ConsoleWnd::CleanupInputCursor()
{
   m_inputCursor.Stop();
}


void ConsoleWnd::UpdateScrollbar()
{
   assert(hwnd());

   SCROLLINFO info;
   std::memset(&info, 0, sizeof(SCROLLINFO));
   info.cbSize = sizeof(SCROLLINFO);
   info.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
   info.nMin = 0;
   info.nMax = m_layout.CountPhysicalLines();
   info.nPage = m_layout.CountVisiblePhysicalLines();
   info.nPos = m_layout.FirstVisiblePhysicalLine();
   ::SetScrollInfo(hwnd(), SB_VERT, &info, true);
}


void ConsoleWnd::DrawBackground(HDC hdc, const win32::Rect& bounds)
{
   FillRect(hdc, &bounds, m_backgroundBrush);
}


void ConsoleWnd::DrawContent(HDC hdc, const win32::Rect& bounds)
{
   // Lazy initialization of layout because we need the DC to calculate the layout
   // metrics. Note that getting a DC before the window is drawn with GetDC() and
   // using that to calculate the layout metrics gives incorrect measurements.
   if (!m_isLayoutInited)
      UpdateContentLayout(hdc);

   const CVINT32 lineHeight = m_layout.LineHeight();
   const std::size_t maxPhysLine = m_layout.CountPhysicalLines() - 1;

   const std::size_t firstDrawnLine =
      m_layout.FirstVisiblePhysicalLine() + bounds.top / lineHeight;
   // Prevent drawing of partial lines at the bottom.
   const std::size_t lastDrawnLine =
      std::min(firstDrawnLine + bounds.height() / lineHeight - 1, maxPhysLine);

   win32::Rect lineBounds = bounds;
   for (std::size_t i = firstDrawnLine; i <= lastDrawnLine; ++i)
   {
      lineBounds.bottom = lineBounds.top + lineHeight;
      if (GetContent().IsEnteredLine(m_layout.LogicalFromPhysicalLine(i)))
         DrawEnteredLine(hdc, lineBounds, m_layout.PhysicalLineText(i));
      else
         DrawLine(hdc, lineBounds, m_layout.PhysicalLineText(i));
      lineBounds.top = lineBounds.bottom;
   }
}


void ConsoleWnd::DrawLine(HDC hdc, win32::Rect bounds, CVString text)
{
   ::DrawTextEx(hdc, const_cast<char*>(text.GetCString()), text.GetSize(), &bounds,
                TextFormatFlags, nullptr);
}


void ConsoleWnd::DrawEnteredLine(HDC hdc, win32::Rect bounds, CVString text)
{
   COLORREF prevColor = SetTextColor(hdc, m_textInputColor);
   DrawLine(hdc, bounds, text);
   SetTextColor(hdc, prevColor);
}


void ConsoleWnd::DrawInputCursor(HDC hdc)
{
   m_inputCursor.Draw(hdc, clientBounds());
}


void ConsoleWnd::MoveInputCursor(CVINT32 offset)
{
   m_layout.MoveInputCursor(offset);
   UpdateInputCursor();
}


void ConsoleWnd::MoveInputCursorToEnd()
{
   m_layout.MoveInputCursorToEnd();
   UpdateInputCursor();
}


bool ConsoleWnd::HandleEditKey(UINT virtKeyCode)
{
   switch (virtKeyCode)
   {
   case VK_LEFT:
      MoveInputCursor(-1);
      return true;
   case VK_RIGHT:
      MoveInputCursor(1);
      return true;
   case VK_UP:
      DisplayPreviousInput();
      return true;
   case VK_DOWN:
      DisplayNextInput();
      return true;
   case VK_RETURN:
      ProcessInputLine();
      return true;
   case VK_BACK:
      DeleteInputCharacter(m_layout.InputCursorPosition() - 1, -1);
      return true;
   case VK_DELETE:
      DeleteInputCharacter(m_layout.InputCursorPosition(), 0);
      return true;
   case VK_PRIOR:
      ScrollVertical(SB_PAGEUP, 0);
      return true;
   case VK_NEXT:
      ScrollVertical(SB_PAGEDOWN, 0);
      return true;
   case VK_HOME:
      MoveInputCursor(-static_cast<CVINT32>(m_layout.InputCursorPosition()));
      return true;
   case VK_END:
      MoveInputCursorToEnd();
      return true;
   case VK_TAB:
      AutoCompleteInput();
      return true;
   }

   return false;
}


bool ConsoleWnd::HandleInputKey(TCHAR ch)
{
   if (ch == VK_BACK || ch == VK_RETURN || ch == VK_TAB)
      return false;

   CVString inputText = GetContent().InputLineText();
   inputText.Insert(m_layout.InputCursorPosition(), ch);
   GetContent().SetInputLine(inputText);

   m_layout.MoveInputCursor(1);
   RefreshInputLine();
   return true;
}


void ConsoleWnd::DeleteInputCharacter(std::size_t charIdx, CVINT32 cursorOffset)
{
   if (charIdx < GetContent().MinInputCursorPosition())
      return;

   InvalInputLine();

   CVString inputText = GetContent().InputLineText();
   inputText.Erase(charIdx);
   GetContent().SetInputLine(inputText);

   m_layout.MoveInputCursor(cursorOffset);
   RefreshInputLine();
}


void ConsoleWnd::ProcessInputLine()
{
   GetContent().ProcessInputLine();
      
   m_layout.CalcContentMetrics(IntRect{clientBounds()});
   m_layout.MoveInputCursor(-static_cast<CVINT32>(m_layout.InputCursorPosition()));

   UpdateScrollbar();
   UpdateInputCursor();
   RefreshInputLine();

   inval(true);
}


void ConsoleWnd::DisplayPreviousInput()
{
   InvalInputLine();
   GetContent().GoToPreviousInput();
   m_layout.MoveInputCursorToEnd();
   RefreshInputLine();
}


void ConsoleWnd::DisplayNextInput()
{
   InvalInputLine();
   GetContent().GoToNextInput();
   m_layout.MoveInputCursorToEnd();
   RefreshInputLine();
}


void ConsoleWnd::AutoCompleteInput()
{
   InvalInputLine();
   GetContent().NextAutoCompletion();
   m_layout.MoveInputCursorToEnd();
   RefreshInputLine();
}


void ConsoleWnd::ScrollVertical(UINT scrollAction, UINT thumbPos)
{
   SCROLLINFO info;
   std::memset(&info, 0, sizeof(SCROLLINFO));
   info.cbSize = sizeof(SCROLLINFO);
   info.fMask = SIF_ALL;
   ::GetScrollInfo(hwnd(), SB_VERT, &info);

   const int prevPos = info.nPos;
   int newPos = prevPos;

   switch (scrollAction)
   {
   case SB_TOP:
      newPos = info.nMin;
      break;
   case SB_BOTTOM:
      newPos = info.nMax;
      break;
   case SB_LINEUP:
      newPos = info.nPos - 1;
      break;
   case SB_LINEDOWN:
      newPos = info.nPos + 1;
      break;
   case SB_PAGEUP:
      newPos = info.nPos - info.nPage;
      break;
   case SB_PAGEDOWN:
      newPos = info.nPos + info.nPage;
      break;
   case SB_THUMBTRACK:
   case SB_THUMBPOSITION:
      newPos = thumbPos;
      break;
   case SB_ENDSCROLL:
      // Nothing to do.
      break;
   default:
      assert(false && "Unknown scroll action.");
      break;
   }

   const bool redraw = (newPos != prevPos);
   ::SetScrollPos(hwnd(), SB_VERT, newPos, redraw);
   // SetScrollPos() checks the passed scroll position and might alter it, if it is
   // outside the [min, max] scroll range. Because of that we have to read it again.
   newPos = ::GetScrollPos(hwnd(), SB_VERT);

   m_layout.SetFirstVisiblePhysicalLine(newPos);
   m_inputCursor.SetTopLeftPosition(win32::Point(m_layout.InputCursorPixelPosition()));

   // Limit the scrolled area to the section of the client area where complete lines are
   // going to be drawn. At the bottom of the client area is a section whose height is
   // less than a line. We want to exclude that area from scrolling.
   const win32::Rect clientArea = clientBounds();
   win32::Rect scrolledBounds = clientArea;
   scrolledBounds.bottom =
      TruncatedDiv<long>(scrolledBounds.height(), m_layout.LineHeight());

   ::ScrollWindowEx(hwnd(), 0, (prevPos - newPos) * m_layout.LineHeight(),
                    &scrolledBounds, nullptr, nullptr, nullptr, SW_ERASE | SW_INVALIDATE);

   // To make sure the bottom section of the client area where no full line fits anymore
   // stays empty, inval and erase it.
   win32::Rect unfilledBottomArea = clientBounds();
   unfilledBottomArea.top = scrolledBounds.bottom;
   inval(unfilledBottomArea, true);
}


void ConsoleWnd::ScrollVerticalBy(int numLines)
{
   int pos = ::GetScrollPos(hwnd(), SB_VERT);
   pos += numLines;
   ScrollVertical(SB_THUMBPOSITION, pos);
}


void ConsoleWnd::ScrollIntoView()
{
   const std::size_t firstVisibleLine = m_layout.FirstVisiblePhysicalLine();
   const std::size_t lastVisibleLine =
      firstVisibleLine + m_layout.CountVisiblePhysicalLines() - 1;

   // Try to have both the line with the cursor and the last input line in the view.
   // But prioritize the cursor line.

   const std::size_t lastInputLine = m_layout.CountPhysicalLines() - 1;
   if (lastInputLine > lastVisibleLine)
      ScrollVerticalBy(lastInputLine - firstVisibleLine);

   const std::size_t cursorLine = m_layout.InputCursorPhysicalLine();
   if (cursorLine < firstVisibleLine)
      ScrollVerticalBy(cursorLine - firstVisibleLine);
}
