//
// Quick config
// Configuration settings.
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "config.h"
#include "storage.h"
#include "essentutils/string_util.h"
#include <cassert>


namespace
{
///////////////////

template <typename Value>
void setSetting(const std::string& key, Value val,
                std::unordered_map<std::string, std::string>& settings)
{
   assert(!key.empty());
   if (key.empty())
      return;

   if constexpr (std::is_same_v<Value, const char*> || std::is_same_v<Value, std::string>)
      settings[key] = val;
   else
      settings[key] = std::to_string(val);
}


template <typename Value>
std::optional<Value>
getSetting(const std::string& key,
           const std::unordered_map<std::string, std::string>& settings)
{
   assert(!key.empty());
   if (key.empty())
      return {};

   const auto entry = settings.find(key);
   if (entry == settings.end())
      return {};

   if constexpr (std::is_floating_point_v<Value>)
      return sutil::fpFromStr<Value>(entry->second);
   else if constexpr (std::is_integral_v<Value>)
      return sutil::intFromStr<Value>(entry->second);
   else
      return entry->second;
}

} // namespace


namespace qcfg
{
///////////////////

bool Config::load(Storage& storage)
{
   clear();
   return storage.load(m_settings);
}


bool Config::save(Storage& storage)
{
   return storage.save(m_settings);
}


void Config::setInt64(const std::string& key, int64_t value)
{
   setSetting(key, value, m_settings);
}


void Config::setInt32(const std::string& key, int32_t value)
{
   setSetting(key, value, m_settings);
}


void Config::setInt16(const std::string& key, int16_t value)
{
   setSetting(key, value, m_settings);
}


void Config::setInt8(const std::string& key, int8_t value)
{
   setSetting(key, value, m_settings);
}


void Config::setFloat(const std::string& key, float value)
{
   setSetting(key, value, m_settings);
}


void Config::setDouble(const std::string& key, double value)
{
   setSetting(key, value, m_settings);
}


void Config::setLongDouble(const std::string& key, long double value)
{
   setSetting(key, value, m_settings);
}


void Config::setBool(const std::string& key, bool value)
{
   setSetting(key, value ? "1" : "0", m_settings);
}


void Config::setString(const std::string& key, const std::string& value)
{
   setSetting(key, value, m_settings);
}


void Config::setWString(const std::string& key, const std::wstring& value)
{
   setSetting(key, sutil::utf8(value), m_settings);
}


std::optional<int64_t> Config::getInt64(const std::string& key) const
{
   return getSetting<int64_t>(key, m_settings);
}


std::optional<int32_t> Config::getInt32(const std::string& key) const
{
   return getSetting<int32_t>(key, m_settings);
}


std::optional<int16_t> Config::getInt16(const std::string& key) const
{
   return getSetting<int16_t>(key, m_settings);
}


std::optional<int8_t> Config::getInt8(const std::string& key) const
{
   return getSetting<int8_t>(key, m_settings);
}


std::optional<float> Config::getFloat(const std::string& key) const
{
   return getSetting<float>(key, m_settings);
}


std::optional<double> Config::getDouble(const std::string& key) const
{
   return getSetting<double>(key, m_settings);
}


std::optional<long double> Config::getLongDouble(const std::string& key) const
{
   return getSetting<long double>(key, m_settings);
}


std::optional<bool> Config::getBool(const std::string& key) const
{
   const auto res = getSetting<std::string>(key, m_settings);
   if (!res.has_value())
      return {};

   const std::string valStr = sutil::trim(sutil::lowercase(res.value()), ' ');
   if (valStr == "1" || valStr == "true" || valStr == "yes")
      return true;
   else if (valStr == "0" || valStr == "false" || valStr == "no")
      return false;
   return {};
}


std::optional<std::string> Config::getString(const std::string& key) const
{
   return getSetting<std::string>(key, m_settings);
}


std::optional<std::wstring> Config::getWString(const std::string& key) const
{
   const auto res = getSetting<std::string>(key, m_settings);
   if (res.has_value())
      return sutil::utf16(res.value());
   return {};
}


void Config::remove(const std::string& key)
{
   auto pos = m_settings.find(key);
   if (pos != m_settings.end())
      m_settings.erase(key);
}


void Config::clear()
{
   m_settings.clear();
}

} // namespace qcfg
