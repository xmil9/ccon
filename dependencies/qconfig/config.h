//
// Quick config
// Configuration settings.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "qconfig_api.h"
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>

namespace qcfg
{
struct Storage;
}


namespace qcfg
{
///////////////////

// Configuration settings.
class QCONFIG_API Config
{
 public:
   bool load(Storage& storage);
   bool save(Storage& storage);

   void setInt64(const std::string& key, int64_t value);
   void setInt32(const std::string& key, int32_t value);
   void setInt16(const std::string& key, int16_t value);
   void setInt8(const std::string& key, int8_t value);
   void setFloat(const std::string& key, float value);
   void setDouble(const std::string& key, double value);
   void setLongDouble(const std::string& key, long double value);
   void setBool(const std::string& key, bool value);
   void setString(const std::string& key, const std::string& value);
   void setWString(const std::string& key, const std::wstring& value);
   std::optional<int64_t> getInt64(const std::string& key) const;
   std::optional<int32_t> getInt32(const std::string& key) const;
   std::optional<int16_t> getInt16(const std::string& key) const;
   std::optional<int8_t> getInt8(const std::string& key) const;
   std::optional<float> getFloat(const std::string& key) const;
   std::optional<double> getDouble(const std::string& key) const;
   std::optional<long double> getLongDouble(const std::string& key) const;
   std::optional<bool> getBool(const std::string& key) const;
   std::optional<std::string> getString(const std::string& key) const;
   std::optional<std::wstring> getWString(const std::string& key) const;
   
   void remove(const std::string& key);
   void clear();

 private:
   std::unordered_map<std::string, std::string> m_settings;
};

} // namespace qcfg
