#include "preferences.h"
#include "util/color.h"

using namespace std;
using namespace util;


namespace
{
///////////////////

struct SuppliedDefaultPref : public ccon::DefaultPrefs
{
   util::Rgb backgroundDefaultColor() const override;
   util::Rgb textOutputDefaultColor() const override;
   util::Rgb textInputDefaultColor() const override;
   int fontSize() const override;
};


Rgb SuppliedDefaultPref::backgroundDefaultColor() const
{
   return Rgb(26, 26, 26);
}


Rgb SuppliedDefaultPref::textOutputDefaultColor() const
{
   return Rgb(230, 230, 230);
}


Rgb SuppliedDefaultPref::textInputDefaultColor() const
{
   return Rgb(255, 211, 149);
}


int SuppliedDefaultPref::fontSize() const
{
   return 11;
}

} // namespace


namespace ccon
{
///////////////////

std::unique_ptr<DefaultPrefs> makeDefaultPrefs()
{
   return make_unique<SuppliedDefaultPref>();
}

} // namespace ccon
