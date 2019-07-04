//
// ccon
//
// Jul-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "win32_util/geometry.h"
#include <cstdint>
#include <string>
#include <vector>

namespace ccon
{
struct ConsoleContent;
}


namespace ccon
{
///////////////////

class ConsoleLayoutWin32
{
 public:
   explicit ConsoleLayoutWin32(ConsoleContent& content);
   ~ConsoleLayoutWin32() = default;
   ConsoleLayoutWin32(const ConsoleLayoutWin32&) = default;
   ConsoleLayoutWin32(ConsoleLayoutWin32&& src) noexcept = default;
   ConsoleLayoutWin32& operator=(const ConsoleLayoutWin32&) = default;
   ConsoleLayoutWin32& operator=(ConsoleLayoutWin32&& src) noexcept = default;

   int lineHeight() const;
   int textHeight() const;
   win32::Rect logicalLineBounds(std::size_t lineIdx) const;
   win32::Rect physicalLineBounds(std::size_t lineIdx) const;
   std::string physicalLineText(std::size_t lineIdx) const;
   bool isInputLine(std::size_t physIdx) const;
   std::size_t logicalFromPhysicalLine(std::size_t physIdx) const;
   std::size_t countPhysicalLines() const;
   std::size_t countVisiblePhysicalLines() const;
   std::size_t firstVisiblePhysicalLine() const;
   void setFirstVisiblePhysicalLine(std::size_t lineIdx);
   std::size_t inputCursorPosition() const;
   win32::Point inputCursorPixelPosition() const;
   std::size_t inputCursorPhysicalLine() const;
   void moveInputCursor(int offset);
   void moveInputCursorToEnd();

   bool calcGeneralTextMetrics(HDC hdc);
   bool calcContentMetrics(const win32::Rect& displayBounds);
   bool calcInputLineMetrics(const win32::Rect& displayBounds);

 private:
   // Metrics for logical content lines. A logical line is a line of entered text
   // until a newline character. The Console interface returns logical lines.
   class LineMetrics
   {
    public:
      LineMetrics() = default;
      LineMetrics(std::size_t firstPhysLineIdx, std::size_t numPhysLines);
      ~LineMetrics() = default;
      LineMetrics(const LineMetrics&) = default;
      LineMetrics(LineMetrics&& src) noexcept = default;
      LineMetrics& operator=(const LineMetrics&) = default;
      LineMetrics& operator=(LineMetrics&& src) noexcept = default;

      std::size_t firstPhysicalLineIndex() const { return m_firstPhysLineIdx; }
      std::size_t countPhysicalLines() const { return m_numPhysLines; }

    private:
      // Index of the first physical line of the logical line.
      std::size_t m_firstPhysLineIdx = 0;
      // Number of physical lines that the logical line occupies.
      std::size_t m_numPhysLines = 0;
   };

   // Metrics for physical content lines. A physical line is a line of displayed text.
   // Logical lines wrap into multiple physical lines depending on the width of the
   // display area.
   class PhysicalLineMetrics
   {
    public:
      PhysicalLineMetrics() = default;
      PhysicalLineMetrics(std::size_t associatedLogLineIdx, const win32::Rect& bounds);
      ~PhysicalLineMetrics() = default;
      PhysicalLineMetrics(const PhysicalLineMetrics&) = default;
      PhysicalLineMetrics(PhysicalLineMetrics&& src) noexcept = default;
      PhysicalLineMetrics& operator=(const PhysicalLineMetrics&) = default;
      PhysicalLineMetrics& operator=(PhysicalLineMetrics&& src) noexcept = default;

      std::size_t logicalLineIndex() const { return m_associatedLogLineIdx; }
      const win32::Rect& bounds() const { return m_bounds; }

    private:
      // Index of the logical line that the physical line is part of.
      std::size_t m_associatedLogLineIdx = 0;
      // Bounds of the line relative to the bounds of the entire content.
      win32::Rect m_bounds;
   };

   ConsoleContent& getContent();
   const ConsoleContent& getContent() const;
   std::size_t maxLogicalIndex() const;
   void calcLineMetrics(std::size_t logLineIdx);
   void removeLineMetrics(std::size_t logLineIdx);
   void removeLineMetrics();
   void calcVisibleLines(const win32::Rect& displayBounds, std::size_t prevNumVisibleLines);
   int calcCursorHorzPosition() const;
   int calcCursorVertPosition() const;

 private:
   std::reference_wrapper<ConsoleContent> m_content;
   int m_lineHeight = 0;
   int m_textHeight = 0;
   int m_charWidth = 0;
   std::size_t m_charsPerLine = 0;
   std::size_t m_firstVisiblePhysLineIdx = 0;
   std::size_t m_numVisiblePhysLines = 0;
   // Metrics for each logical content line. Associated to logical lines by index.
   std::vector<LineMetrics> m_logMetrics;
   // The console content split into physical lines of text each fitting into the
   // available display width.
   std::vector<std::string> m_physLines;
   // Metrics for each physical content line. Associated to physical lines by index.
   std::vector<PhysicalLineMetrics> m_physMetrics;
   // Zero-based character index of input cursor on logical input line.
   std::size_t m_cursorPos = 0;
};


inline int ConsoleLayoutWin32::lineHeight() const
{
   return m_lineHeight;
}

inline int ConsoleLayoutWin32::textHeight() const
{
   return m_textHeight;
}

inline std::size_t ConsoleLayoutWin32::countPhysicalLines() const
{
   return m_physLines.size();
}

inline std::size_t ConsoleLayoutWin32::firstVisiblePhysicalLine() const
{
   return m_firstVisiblePhysLineIdx;
}

inline std::size_t ConsoleLayoutWin32::inputCursorPosition() const
{
   return m_cursorPos;
}

inline ConsoleContent& ConsoleLayoutWin32::getContent()
{
   return m_content.get();
}

inline const ConsoleContent& ConsoleLayoutWin32::getContent() const
{
   return m_content.get();
}

} // namespace ccon

#endif // _WIN32
