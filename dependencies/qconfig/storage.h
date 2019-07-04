//
// Quick config
// Configuration storage abstraction.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "qconfig_api.h"
#include <string>
#include <unordered_map>


namespace qcfg
{
///////////////////

// Abstraction for storing config settings.
struct QCONFIG_API Storage
{
   using Settings = std::unordered_map<std::string, std::string>;

   virtual ~Storage() = default;
   virtual bool load(Settings& settings) = 0;
   virtual bool save(const Settings& settings) = 0;
};

} // namespace qcfg
