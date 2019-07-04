#include "preferences.h"

using namespace std;


namespace
{
///////////////////

const CVString ConsoleWidthTag = "ConsoleWidth";
const CVString ConsoleHeightTag = "ConsoleHeight";
const CVString BackgroundColorTag = "BackgroundColor";
const CVString TextOutputColorTag = "TextOutputColor";
const CVString TextInputColorTag = "TextInputColor";
const CVString FontSizeTag = "FontSize";


///////////////////

const CVString RedTag = "_R";
const CVString GreenTag = "_G";
const CVString BlueTag = "_B";


void StoreColor(CVINT32 toolId, const CVString& tag, const Rgb& color)
{
   CVSetAppData_Integer(toolId, tag + RedTag, color.r);
   CVSetAppData_Integer(toolId, tag + GreenTag, color.g);
   CVSetAppData_Integer(toolId, tag + BlueTag, color.b);
}


pair<bool, Rgb> FetchColor(CVINT32 toolId, const CVString& tag)
{
   Rgb color;
   CVINT32 val = 0;

   if (!CVGetAppData_Integer(toolId, tag + RedTag, &val))
      return {false, {}};
   color.r = val;

   if (!CVGetAppData_Integer(toolId, tag + GreenTag, &val))
      return {false, {}};
   color.g = val;


   if (!CVGetAppData_Integer(toolId, tag + BlueTag, &val))
      return {false, {}};
   color.b = val;

   return {true, color};
}

} // namespace


///////////////////

ConsoleUIUserPrefs::ConsoleUIUserPrefs(TToolID toolId) : m_toolId{toolId}
{
}


std::pair<bool, CVINT32> ConsoleUIUserPrefs::ConsoleWidth() const
{
   CVINT32 val = 0;
   if (CVGetAppData_Integer(m_toolId, ConsoleWidthTag, &val))
      return {true, val};
   return {false, 0};
}


void ConsoleUIUserPrefs::SetConsoleWidth(CVINT32 w)
{
   CVSetAppData_Integer(m_toolId, ConsoleWidthTag, w);
}


std::pair<bool, CVINT32> ConsoleUIUserPrefs::ConsoleHeight() const
{
   CVINT32 val = 0;
   if (CVGetAppData_Integer(m_toolId, ConsoleHeightTag, &val))
      return {true, val};
   return {false, 0};
}


void ConsoleUIUserPrefs::SetConsoleHeight(CVINT32 h)
{
   CVSetAppData_Integer(m_toolId, ConsoleHeightTag, h);
}


pair<bool, Rgb> ConsoleUIUserPrefs::BackgroundColor() const
{
   return FetchColor(m_toolId, BackgroundColorTag);
}


void ConsoleUIUserPrefs::SetBackgroundColor(const Rgb& color)
{
   StoreColor(m_toolId, BackgroundColorTag, color);
}


pair<bool, Rgb> ConsoleUIUserPrefs::TextOutputColor() const
{
   return FetchColor(m_toolId, TextOutputColorTag);
}


void ConsoleUIUserPrefs::SetTextOutputColor(const Rgb& color)
{
   StoreColor(m_toolId, TextOutputColorTag, color);
}


pair<bool, Rgb> ConsoleUIUserPrefs::TextInputColor() const
{
   return FetchColor(m_toolId, TextInputColorTag);
}


void ConsoleUIUserPrefs::SetTextInputColor(const Rgb& color)
{
   StoreColor(m_toolId, TextInputColorTag, color);
}


std::pair<bool, int> ConsoleUIUserPrefs::FontSize() const
{
   CVINT32 val = 0;
   if (CVGetAppData_Integer(m_toolId, FontSizeTag, &val))
      return {true, val};
   return {false, 0};
}


void ConsoleUIUserPrefs::SetFontSize(int sizeInPoints)
{
   CVSetAppData_Integer(m_toolId, FontSizeTag, sizeInPoints);
}


namespace
{
///////////////////

class StandardUserPrefs : public UserPrefs
{
public:
   bool load() override;
   bool save() override;
   std::optional<int> consoleWidth() const override;
   std::optional<int> consoleHeight() const override;
   std::optional<util::Rgb> backgroundColor() const override;
   std::optional<util::Rgb> textOutputColor() const override;
   std::optional<util::Rgb> textInputColor() const override;
   std::optional<int> fontSize() const override;
   void setConsoleWidth(int w) override;
   void setConsoleHeight(int h) override;
   void setBackgroundColor(const util::Rgb& color) override;
   void setTextOutputColor(const util::Rgb& color) override;
   void setTextInputColor(const util::Rgb& color) override;
   void setFontSize(int sizeInPoints) override;

private:
}

} // namespace


namespace ccon
{
///////////////////

std::unique_ptr<UserPrefs> makeUserPrefs()
{
   return make_unique<StandardUserPrefs>();
}

} // namespace ccon
