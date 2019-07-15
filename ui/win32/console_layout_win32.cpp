//
// ccon
//
// Jul-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "console_layout_win32.h"
#include "console_content.h"
#include <algorithm>
#include <cassert>
#include <vector>


namespace
{

///////////////////

std::vector<std::string> splitText(const std::string& text, std::size_t charsPerLine)
{
   std::vector<std::string> lines;
   size_t pos = 0;
   size_t size = text.size();
   while (size > charsPerLine)
   {
      lines.push_back(text.substr(pos, charsPerLine));
      pos += charsPerLine;
      size -= charsPerLine;
   }

   if (size != 0)
      lines.push_back(text.substr(pos));

   return lines;
}

} // namespace


namespace ccon
{
///////////////////

ConsoleLayoutWin32::ConsoleLayoutWin32(ConsoleContent& content)
: m_content{content}, m_cursorPos{
                         static_cast<std::size_t>(content.inputLineText().size())}
{
}


win32::Rect ConsoleLayoutWin32::logicalLineBounds(std::size_t lineIdx) const
{
   const LineMetrics& logMetrics = m_logMetrics[lineIdx];
   const std::size_t firstPhysLine = logMetrics.firstPhysicalLineIndex();
   const std::size_t lastPhysLine = firstPhysLine + logMetrics.countPhysicalLines() - 1;

   win32::Rect bounds = m_physMetrics[firstPhysLine].bounds();
   for (std::size_t i = firstPhysLine + 1; i <= lastPhysLine; ++i)
      bounds = unite(bounds, m_physMetrics[i].bounds());

   // Map bounds to be relative to the visible area.
   bounds.offset(0, -m_physMetrics[m_firstVisiblePhysLineIdx].bounds().top);

   return bounds;
}


win32::Rect ConsoleLayoutWin32::physicalLineBounds(std::size_t lineIdx) const
{
   win32::Rect bounds = m_physMetrics[lineIdx].bounds();
   // Map bounds to be relative to the visible area.
   bounds.offset(0, -m_physMetrics[m_firstVisiblePhysLineIdx].bounds().top);
   return bounds;
}


std::string ConsoleLayoutWin32::physicalLineText(std::size_t lineIdx) const
{
   assert(lineIdx < countPhysicalLines());
   return m_physLines[lineIdx];
}


bool ConsoleLayoutWin32::isInputLine(std::size_t physIdx) const
{
   const LineMetrics& inputLineMetrics = m_logMetrics[maxLogicalIndex()];
   const std::size_t firstLine = inputLineMetrics.firstPhysicalLineIndex();
   return (firstLine <= physIdx &&
           physIdx < firstLine + inputLineMetrics.countPhysicalLines());
}


std::size_t ConsoleLayoutWin32::logicalFromPhysicalLine(std::size_t physIdx) const
{
   assert(physIdx < countPhysicalLines());
   return m_physMetrics[physIdx].logicalLineIndex();
}


std::size_t ConsoleLayoutWin32::countVisiblePhysicalLines() const
{
   return m_numVisiblePhysLines;
}


void ConsoleLayoutWin32::setFirstVisiblePhysicalLine(std::size_t lineIdx)
{
   m_firstVisiblePhysLineIdx = lineIdx;
}


win32::Point ConsoleLayoutWin32::inputCursorPixelPosition() const
{
   return {calcCursorHorzPosition(), calcCursorVertPosition()};
}


std::size_t ConsoleLayoutWin32::inputCursorPhysicalLine() const
{
   const LineMetrics& inputLineMetrics = m_logMetrics[maxLogicalIndex()];
   const std::size_t inputLineInternalIdx = m_cursorPos / m_charsPerLine;
   return inputLineMetrics.firstPhysicalLineIndex() + inputLineInternalIdx;
}


void ConsoleLayoutWin32::moveInputCursor(int offset)
{
   size_t newPos = m_cursorPos + offset;

   if (newPos < getContent().minInputCursorPosition())
      newPos = getContent().minInputCursorPosition();

   const size_t maxPos = getContent().inputLineText().size();
   if (newPos > maxPos)
      newPos = maxPos;

   m_cursorPos = newPos;
}


void ConsoleLayoutWin32::moveInputCursorToEnd()
{
   m_cursorPos = getContent().inputLineText().size();
}


bool ConsoleLayoutWin32::calcGeneralTextMetrics(HDC hdc)
{
   TEXTMETRIC metrics;
   if (!::GetTextMetrics(hdc, &metrics))
      return false;

   // Source:
   // https://docs.microsoft.com/en-us/windows/desktop/gdi/string-widths-and-heights
   m_lineHeight = metrics.tmHeight + metrics.tmExternalLeading;
   m_textHeight = metrics.tmHeight;
   m_charWidth = metrics.tmAveCharWidth;

   return true;
}


bool ConsoleLayoutWin32::calcContentMetrics(const win32::Rect& displayBounds)
{
   const std::size_t prevNumVisibleLines = m_numVisiblePhysLines;
   m_charsPerLine = displayBounds.width() / m_charWidth;

   removeLineMetrics();

   const std::size_t numLines = getContent().countLines();
   for (std::size_t i = 0; i < numLines; ++i)
      calcLineMetrics(i);

   calcVisibleLines(displayBounds, prevNumVisibleLines);

   return true;
}


bool ConsoleLayoutWin32::calcInputLineMetrics(const win32::Rect& displayBounds)
{
   const std::size_t inputLineIdx = maxLogicalIndex();

   removeLineMetrics(inputLineIdx);
   calcLineMetrics(inputLineIdx);

   return true;
}


std::size_t ConsoleLayoutWin32::maxLogicalIndex() const
{
   // There should always at least be the input line.
   assert(!m_logMetrics.empty());
   return m_logMetrics.size() - 1;
}


void ConsoleLayoutWin32::calcLineMetrics(std::size_t logLineIdx)
{
   const std::vector<std::string> splitLines =
      splitText(getContent().lineText(logLineIdx), m_charsPerLine);

   m_logMetrics[logLineIdx] = LineMetrics{m_physLines.size(), splitLines.size()};

   std::copy(splitLines.begin(), splitLines.end(), back_inserter(m_physLines));

   const long top = static_cast<long>(m_physMetrics.size()) * m_lineHeight;
   win32::Rect physLineBounds{0, top, 0, 0};
   for (const std::string& text : splitLines)
   {
      physLineBounds.right = static_cast<long>(text.size()) * m_charWidth;
      physLineBounds.bottom = physLineBounds.top + m_lineHeight;

      m_physMetrics.emplace_back(logLineIdx, physLineBounds);

      physLineBounds.top = physLineBounds.bottom;
   }
}


void ConsoleLayoutWin32::removeLineMetrics(std::size_t logLineIdx)
{
   const std::size_t physStartIdx = m_logMetrics[logLineIdx].firstPhysicalLineIndex();
   const std::size_t physEndIdx =
      physStartIdx + m_logMetrics[logLineIdx].countPhysicalLines();

   m_logMetrics[logLineIdx] = {};
   m_physLines.erase(m_physLines.begin() + physStartIdx,
                     m_physLines.begin() + physEndIdx);
   m_physMetrics.erase(m_physMetrics.begin() + physStartIdx,
                       m_physMetrics.begin() + physEndIdx);
}


void ConsoleLayoutWin32::removeLineMetrics()
{
   m_logMetrics.resize(getContent().countLines(), {});
   m_physLines.clear();
   m_physMetrics.clear();
}


void ConsoleLayoutWin32::calcVisibleLines(const win32::Rect& displayBounds,
                                          std::size_t prevNumVisibleLines)
{
   m_numVisiblePhysLines = displayBounds.height() / m_lineHeight;

   if (prevNumVisibleLines != m_numVisiblePhysLines)
   {
      const bool canDisplayAll = (m_numVisiblePhysLines >= m_physLines.size());
      const bool isBottomLineVisible =
         (m_firstVisiblePhysLineIdx + m_numVisiblePhysLines >= m_physLines.size());

      if (canDisplayAll)
      {
         m_firstVisiblePhysLineIdx = 0;
      }
      else if (isBottomLineVisible)
      {
         // If the bottom line is visible, keep it visible and show/hide lines at the top
         // of the window.
         m_firstVisiblePhysLineIdx =
            std::max<std::size_t>(m_physLines.size() - m_numVisiblePhysLines, 0);
      }
      else
      {
         // If the bottom line is not visible, show/hide lines at the bottom of the
         // window. Also, adjusting the bottom lines makes for a smoother resize
         // experience because less content has to redraw, so it makes sense to prioritize
         // it over adjusting the top lines when there is a choice.
         // Nothing to do. This happens automatically by re-calculating the number
         // of visible lines above.
      }
   }
}


int ConsoleLayoutWin32::calcCursorHorzPosition() const
{
   const std::size_t physCharIdx = m_cursorPos % m_charsPerLine;
   return static_cast<int>(physCharIdx) * m_charWidth;
}


int ConsoleLayoutWin32::calcCursorVertPosition() const
{
   return static_cast<int>(inputCursorPhysicalLine() - m_firstVisiblePhysLineIdx) *
          m_lineHeight;
}


///////////////////

ConsoleLayoutWin32::LineMetrics::LineMetrics(std::size_t firstPhysLineIdx,
                                             std::size_t numPhysLines)
: m_firstPhysLineIdx{firstPhysLineIdx}, m_numPhysLines{numPhysLines}
{
}


ConsoleLayoutWin32::PhysicalLineMetrics::PhysicalLineMetrics(
   std::size_t associatedLogLineIdx, const win32::Rect& bounds)
: m_associatedLogLineIdx{associatedLogLineIdx}, m_bounds{bounds}
{
}

} // namespace ccon

#endif // _WIN32
