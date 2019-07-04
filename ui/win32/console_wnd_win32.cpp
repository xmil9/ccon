//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "console_wnd_win32.h"
#include "essentutils/color.h"
#include "essentutils/string_util.h"
#include "win32_util/gdi_object.h"
#include "win32_util/screen.h"
#include "win32_util/win32_windows.h"
#include <tchar.h>
#include <cassert>
#include <cwchar>

using namespace std;
using namespace sutil;
using namespace win32;


namespace
{

///////////////////

constexpr UINT TextFormatFlags = DT_LEFT | DT_TOP | DT_NOPREFIX | DT_EXTERNALLEADING;
constexpr UINT CursorBlinkRateMs = 500;
constexpr long CursorWidth = 1;


///////////////////

// Converts a point size (1/72 of an inch) to a raw pixel size.
int pointsToPixels(int points)
{
   return MulDiv(points, vertScreenDpi(), 72);
}


template <typename Int> Int truncatedDiv(Int divident, Int divisor)
{
   static_assert(std::is_integral_v<Int> && !std::is_same_v<Int, bool>,
                 "Integral type expected.");
   return (divident / divisor) * divisor;
}


template <typename Int> Int truncatedDivRest(Int divident, Int divisor)
{
   static_assert(std::is_integral_v<Int> && !std::is_same_v<Int, bool>,
                 "Integral type expected.");
   return (divident - TruncatedDiv(divident, divisor));
}


optional<char> toNarrowChar(TCHAR tch)
{
   // The narrow char might be multiple bytes long. It that case we ignore it.
   const string mbCh = utf8(tch);
   if (mbCh.size() == 1)
      return mbCh[0];
   return nullopt;
}


COLORREF colorrefFromRgb(const Rgb& rgb)
{
   return RGB(rgb.r, rgb.g, rgb.b);
}


GdiObj<HFONT> makeConsoleFont(int heightInPoints)
{
   LOGFONT lf;
   wcsncpy(lf.lfFaceName, L"Consolas", _countof(lf.lfFaceName));
   lf.lfHeight = -pointsToPixels(heightInPoints);
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


namespace ccon
{
///////////////////

ConsoleWndWin32::ConsoleWndWin32(const UserPrefs& prefs, ConsoleContent& content)
: m_userPrefs{prefs}, m_content{content}, m_layout{content}
{
}


void ConsoleWndWin32::setBackgroundColor(const Rgb& color)
{
   m_backgroundBrush =
      move(GdiObj<HBRUSH>(CreateSolidBrush(RGB(color.r, color.g, color.b))));
   inval(true);

   m_userPrefs.setBackgroundColor(color);
}


void ConsoleWndWin32::setOutputTextColor(const Rgb& color)
{
   m_textOutputColor = RGB(color.r, color.g, color.b);
   inval(true);

   m_userPrefs.setTextOutputColor(color);
}


void ConsoleWndWin32::setInputTextColor(const Rgb& color)
{
   m_textInputColor = RGB(color.r, color.g, color.b);
   inval(true);

   m_userPrefs.setTextInputColor(color);
}


void ConsoleWndWin32::resetColors()
{
   setBackgroundColor(DefaultPrefs::backgroundDefaultColor());
   setOutputTextColor(DefaultPrefs::textOutputDefaultColor());
   setInputTextColor(DefaultPrefs::textInputDefaultColor());
}


void ConsoleWndWin32::setFontSize(int sizeInPoints)
{
   m_font = makeConsoleFont(sizeInPoints);

   // Changing the font size requires the entire layout to be recalculated.
   m_isLayoutInited = false;
   inval(true);

   m_userPrefs.setFontSize(sizeInPoints);
}


const TCHAR* ConsoleWndWin32::windowClassName() const
{
   return _T("ConsoleWndClass");
}


bool ConsoleWndWin32::registerWindowClass() const
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


Window::CreationResult ConsoleWndWin32::onCreate(const CREATESTRUCT* createInfo)
{
   Window::onCreate(createInfo);

   if (!setupColors())
      return CreationResult::Abort;
   if (!setupFont())
      return CreationResult::Abort;
   if (!setupBackgroundBrush())
      return CreationResult::Abort;

   return CreationResult::Handled;
}


bool ConsoleWndWin32::onDestroy()
{
   cleanupInputCursor();

   Window::onDestroy();
   return true;
}


bool ConsoleWndWin32::onPaint()
{
   // Only draw if there is an area that needs updating.
   if (haveInvalBounds())
   {
      win32::PaintDC pdc{hwnd()};
      if (pdc)
      {
         ConsoleDCAttributes dcAttribs{pdc, m_font, m_textOutputColor};

         if (pdc.paintInfo().fErase)
            drawBackground(pdc, pdc.paintInfo().rcPaint);

         drawContent(pdc.hdc(), pdc.paintInfo().rcPaint);
         drawInputCursor(pdc.hdc());
      }
   }

   return true;
}


bool ConsoleWndWin32::onEraseBkgnd(HDC hdc)
{
   // We draw the background in the paint handler.
   return false;
}


bool ConsoleWndWin32::onSize(long width, long height, UINT resizeFlag)
{
   if (!m_isLayoutInited)
      return false;

   m_layout.calcContentMetrics(clientBounds());
   updateScrollbar();
   updateInputCursor();
   inval(true);

   m_userPrefs.setConsoleWidth(width);
   m_userPrefs.setConsoleHeight(height);
   return true;
}


bool ConsoleWndWin32::onKeyDown(UINT virtKeyCode, UINT repeatCount, BYTE scanCode,
                                bool isExtendedKey, bool wasPreviouslyDown)
{
   return handleEditKey(virtKeyCode);
}


bool ConsoleWndWin32::onChar(TCHAR ch, UINT repeatCount, BYTE scanCode,
                             bool isExtendedKey, bool wasPreviouslyDown, bool isAltDown,
                             bool isReleased)
{
   return handleInputKey(ch);
}


bool ConsoleWndWin32::onSetFocus(HWND unfocusedWnd)
{
   m_inputCursor.start();
   return true;
}


bool ConsoleWndWin32::onKillFocus(HWND focusedWnd)
{
   m_inputCursor.stop();
   return true;
}


bool ConsoleWndWin32::onVScroll(UINT scrollAction, UINT thumbPos, HWND scrollCtrl)
{
   scrollVertical(scrollAction, thumbPos);
   return true;
}


bool ConsoleWndWin32::onMouseWheel(int delta, UINT keyState, win32::Point mousePos)
{
   // The passed delta value could be smaller than the wheel delta (the threshold for
   // wheel scrolling. If delta is smaller then we have to accumalate it until it reaches
   // the threshold.
   static int accumulatedDelta = 0;

   constexpr UINT kDefaultNumLines = 3;
   UINT numLinesPerWheelDelta = kDefaultNumLines;
   if (!SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &numLinesPerWheelDelta, 0))
      numLinesPerWheelDelta = kDefaultNumLines;

   accumulatedDelta += delta;
   const bool scrollUp = accumulatedDelta > 0;

   // The passed delta could also be (multiple times) larger than the wheel delta.
   // Process large deltas iteratively as individual scrolls.
   while (std::abs(accumulatedDelta) >= WHEEL_DELTA)
   {
      scrollVerticalBy(scrollUp ? -static_cast<int>(numLinesPerWheelDelta)
                                : numLinesPerWheelDelta);
      accumulatedDelta += scrollUp ? -WHEEL_DELTA : WHEEL_DELTA;
   }

   return true;
}


bool ConsoleWndWin32::setupColors()
{
   m_textOutputColor = colorrefFromRgb(DefaultPrefs::textOutputDefaultColor());
   optional<Rgb> color = m_userPrefs.textOutputColor();
   if (color.has_value())
      m_textOutputColor = RGB(color.value().r, color.value().g, color.value().b);

   m_textInputColor = colorrefFromRgb(DefaultPrefs::textInputDefaultColor());
   color = m_userPrefs.textInputColor();
   if (color.has_value())
      m_textInputColor = RGB(color.value().r, color.value().g, color.value().b);

   return true;
}


bool ConsoleWndWin32::setupFont()
{
   optional<int> fontSize = m_userPrefs.fontSize();
   if (!fontSize.has_value())
      fontSize = DefaultPrefs::fontSize();

   setFontSize(fontSize.value());
   return !!m_font;
}


bool ConsoleWndWin32::setupBackgroundBrush()
{
   optional<Rgb> color = m_userPrefs.backgroundColor();
   if (!color.has_value())
      color = DefaultPrefs::backgroundDefaultColor();

   setBackgroundColor(color.value());
   return !!m_backgroundBrush;
}


void ConsoleWndWin32::updateContentLayout(HDC hdc)
{
   m_layout.calcGeneralTextMetrics(hdc);
   m_layout.calcContentMetrics(clientBounds());
   m_isLayoutInited = true;

   // Changes to the layout affect other parts of the UI.
   updateScrollbar();
   updateInputCursor();
}


void ConsoleWndWin32::updateInputLine(HDC hdc)
{
   const std::size_t prevNumLines = m_layout.countPhysicalLines();
   m_layout.calcInputLineMetrics(clientBounds());

   const std::size_t numLines = m_layout.countPhysicalLines();
   if (numLines != prevNumLines)
      updateScrollbar();
   scrollIntoView();
   updateInputCursor();
}


void ConsoleWndWin32::invalInputLine()
{
   const Rect bounds = m_layout.logicalLineBounds(m_content.countLines() - 1);
   inval(bounds, true);
}


void ConsoleWndWin32::refreshInputLine()
{
   SharedDC consoleDC{GetDC(hwnd()), hwnd()};
   if (consoleDC)
   {
      ConsoleDCAttributes dcAttribs{consoleDC, m_font, m_textOutputColor};

      updateInputLine(consoleDC);
      invalInputLine();
   }
}


void ConsoleWndWin32::updateInputCursor()
{
   m_inputCursor.stop();

   m_inputCursor = move(ConsoleInputCursorWin32{hwnd(), m_textInputColor});
   m_inputCursor.setBlinkRate(CursorBlinkRateMs);
   m_inputCursor.setWidth(CursorWidth);
   m_inputCursor.setHeight(m_layout.textHeight());
   m_inputCursor.setTopLeftPosition(m_layout.inputCursorPixelPosition());
   m_inputCursor.start();
}


void ConsoleWndWin32::cleanupInputCursor()
{
   m_inputCursor.stop();
}


void ConsoleWndWin32::updateScrollbar()
{
   assert(hwnd());

   SCROLLINFO info;
   std::memset(&info, 0, sizeof(SCROLLINFO));
   info.cbSize = sizeof(SCROLLINFO);
   info.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
   info.nMin = 0;
   info.nMax = static_cast<int>(m_layout.countPhysicalLines());
   info.nPage = static_cast<int>(m_layout.countVisiblePhysicalLines());
   info.nPos = static_cast<int>(m_layout.firstVisiblePhysicalLine());
   SetScrollInfo(hwnd(), SB_VERT, &info, true);
}


void ConsoleWndWin32::drawBackground(HDC hdc, const win32::Rect& bounds)
{
   FillRect(hdc, &bounds, m_backgroundBrush);
}


void ConsoleWndWin32::drawContent(HDC hdc, const win32::Rect& bounds)
{
   // Lazy initialization of layout because we need the DC to calculate the layout
   // metrics. Note that getting a DC before the window is drawn with GetDC() and
   // using that to calculate the layout metrics gives incorrect measurements.
   if (!m_isLayoutInited)
      updateContentLayout(hdc);

   const int lineHeight = m_layout.lineHeight();
   const size_t maxPhysLine = m_layout.countPhysicalLines() - 1;

   const size_t firstDrawnLine =
      m_layout.firstVisiblePhysicalLine() + bounds.top / lineHeight;
   // Prevent drawing of partial lines at the bottom.
   const size_t lastDrawnLine =
      min(firstDrawnLine + bounds.height() / lineHeight - 1, maxPhysLine);

   Rect lineBounds = bounds;
   for (size_t i = firstDrawnLine; i <= lastDrawnLine; ++i)
   {
      lineBounds.bottom = lineBounds.top + lineHeight;
      if (m_content.isEnteredLine(m_layout.logicalFromPhysicalLine(i)))
         drawEnteredLine(hdc, lineBounds, m_layout.physicalLineText(i));
      else
         drawLine(hdc, lineBounds, m_layout.physicalLineText(i));
      lineBounds.top = lineBounds.bottom;
   }
}


void ConsoleWndWin32::drawLine(HDC hdc, win32::Rect bounds, std::string text)
{
   DrawTextExA(hdc, const_cast<char*>(text.data()), static_cast<int>(text.size()),
               &bounds, TextFormatFlags, nullptr);
}


void ConsoleWndWin32::drawEnteredLine(HDC hdc, win32::Rect bounds,
                                      const std::string& text)
{
   COLORREF prevColor = SetTextColor(hdc, m_textInputColor);
   drawLine(hdc, bounds, text);
   SetTextColor(hdc, prevColor);
}


void ConsoleWndWin32::drawInputCursor(HDC hdc)
{
   m_inputCursor.draw(hdc, clientBounds());
}


void ConsoleWndWin32::moveInputCursor(int offset)
{
   m_layout.moveInputCursor(offset);
   updateInputCursor();
}


void ConsoleWndWin32::moveInputCursorToEnd()
{
   m_layout.moveInputCursorToEnd();
   updateInputCursor();
}


bool ConsoleWndWin32::handleEditKey(UINT virtKeyCode)
{
   switch (virtKeyCode)
   {
   case VK_LEFT:
      moveInputCursor(-1);
      return true;
   case VK_RIGHT:
      moveInputCursor(1);
      return true;
   case VK_UP:
      displayPreviousInput();
      return true;
   case VK_DOWN:
      displayNextInput();
      return true;
   case VK_RETURN:
      processInputLine();
      return true;
   case VK_BACK:
      deleteInputCharacter(m_layout.inputCursorPosition() - 1, -1);
      return true;
   case VK_DELETE:
      deleteInputCharacter(m_layout.inputCursorPosition(), 0);
      return true;
   case VK_PRIOR:
      scrollVertical(SB_PAGEUP, 0);
      return true;
   case VK_NEXT:
      scrollVertical(SB_PAGEDOWN, 0);
      return true;
   case VK_HOME:
      moveInputCursor(-static_cast<int>(m_layout.inputCursorPosition()));
      return true;
   case VK_END:
      moveInputCursorToEnd();
      return true;
   case VK_TAB:
      autoCompleteInput();
      return true;
   }

   return false;
}


bool ConsoleWndWin32::handleInputKey(TCHAR tch)
{
   if (tch == VK_BACK || tch == VK_RETURN || tch == VK_TAB)
      return false;

   const optional<char> ch = toNarrowChar(tch);
   if (!ch.has_value())
      return false;

   string inputText = m_content.inputLineText();
   inputText.insert(inputText.begin() + m_layout.inputCursorPosition(), ch.value());
   m_content.setInputLine(inputText);

   m_layout.moveInputCursor(1);
   refreshInputLine();
   return true;
}


void ConsoleWndWin32::deleteInputCharacter(std::size_t charIdx, int cursorOffset)
{
   if (charIdx < m_content.minInputCursorPosition())
      return;

   invalInputLine();

   string inputText = m_content.inputLineText();
   inputText.erase(charIdx);
   m_content.setInputLine(inputText);

   m_layout.moveInputCursor(cursorOffset);
   refreshInputLine();
}


void ConsoleWndWin32::processInputLine()
{
   m_content.processInputLine();

   m_layout.calcContentMetrics(clientBounds());
   m_layout.moveInputCursor(-static_cast<int>(m_layout.inputCursorPosition()));

   updateScrollbar();
   updateInputCursor();
   refreshInputLine();

   inval(true);
}


void ConsoleWndWin32::displayPreviousInput()
{
   invalInputLine();
   m_content.goToPreviousInput();
   m_layout.moveInputCursorToEnd();
   refreshInputLine();
}


void ConsoleWndWin32::displayNextInput()
{
   invalInputLine();
   m_content.goToNextInput();
   m_layout.moveInputCursorToEnd();
   refreshInputLine();
}


void ConsoleWndWin32::autoCompleteInput()
{
   invalInputLine();
   m_content.nextAutoCompletion();
   m_layout.moveInputCursorToEnd();
   refreshInputLine();
}


void ConsoleWndWin32::scrollVertical(UINT scrollAction, UINT thumbPos)
{
   SCROLLINFO info;
   std::memset(&info, 0, sizeof(SCROLLINFO));
   info.cbSize = sizeof(SCROLLINFO);
   info.fMask = SIF_ALL;
   GetScrollInfo(hwnd(), SB_VERT, &info);

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
   SetScrollPos(hwnd(), SB_VERT, newPos, redraw);
   // SetScrollPos() checks the passed scroll position and might alter it, if it is
   // outside the [min, max] scroll range. Because of that we have to read it again.
   newPos = GetScrollPos(hwnd(), SB_VERT);

   m_layout.setFirstVisiblePhysicalLine(newPos);
   m_inputCursor.setTopLeftPosition(m_layout.inputCursorPixelPosition());

   // Limit the scrolled area to the section of the client area where complete lines are
   // going to be drawn. At the bottom of the client area is a section whose height is
   // less than a line. We want to exclude that area from scrolling.
   const Rect clientArea = clientBounds();
   Rect scrolledBounds = clientArea;
   scrolledBounds.bottom =
      truncatedDiv<long>(scrolledBounds.height(), m_layout.lineHeight());

   ScrollWindowEx(hwnd(), 0, (prevPos - newPos) * m_layout.lineHeight(), &scrolledBounds,
                  nullptr, nullptr, nullptr, SW_ERASE | SW_INVALIDATE);

   // To make sure the bottom section of the client area where no full line fits anymore
   // stays empty, inval and erase it.
   Rect unfilledBottomArea = clientBounds();
   unfilledBottomArea.top = scrolledBounds.bottom;
   inval(unfilledBottomArea, true);
}


void ConsoleWndWin32::scrollVerticalBy(int numLines)
{
   int pos = GetScrollPos(hwnd(), SB_VERT);
   pos += numLines;
   scrollVertical(SB_THUMBPOSITION, pos);
}


void ConsoleWndWin32::scrollIntoView()
{
   const size_t firstVisibleLine = m_layout.firstVisiblePhysicalLine();
   const size_t lastVisibleLine =
      firstVisibleLine + m_layout.countVisiblePhysicalLines() - 1;

   // Try to have both the line with the cursor and the last input line in the view.
   // But prioritize the cursor line.

   const size_t lastInputLine = m_layout.countPhysicalLines() - 1;
   if (lastInputLine > lastVisibleLine)
      scrollVerticalBy(static_cast<int>(lastInputLine - firstVisibleLine));

   const size_t cursorLine = m_layout.inputCursorPhysicalLine();
   if (cursorLine < firstVisibleLine)
      scrollVerticalBy(static_cast<int>(cursorLine - firstVisibleLine));
}

} // namespace ccon

#endif // _WIN32
