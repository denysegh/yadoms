#include "stdafx.h"
#include "SupportedPlatformsChecker.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/versioning/Version.h>
#include "tools/OperatingSystem.h"

namespace tools
{

   CSupportedPlatformsChecker::CSupportedPlatformsChecker()
   {
   }

   CSupportedPlatformsChecker::~CSupportedPlatformsChecker()
   {
   }

   bool CSupportedPlatformsChecker::isSupported(const shared::CDataContainer& supportedPlatformsNode)
   {
      if (supportedPlatformsNode.containsValue(""))
      {
         if (supportedPlatformsNode.getString("") == "all")
            return true;

         throw shared::exception::CInvalidParameter("Error reading package.json : supported platforms is invalid");
      }

      if (supportedPlatformsNode.exists(COperatingSystem::getName()))
      {
         // Our current platform is explicitly given in the configuration
         const shared::CDataContainer& value = supportedPlatformsNode.getChild(COperatingSystem::getName());
         if (!value.containsValue(""))
            return isVersionSupported(value);
         if (value.getString("") == "supported")
            return true;
         if (value.getString("") == "unsupported")
            return false;

         throw shared::exception::CInvalidParameter("Error reading package.json : supported platforms is invalid");
      }

      // Our current platform is not explicitly given in the configuration
      if (supportedPlatformsNode.exists("others"))
      {
         // Our current platform is explicitly given in the configuration
         const std::string& value = supportedPlatformsNode.getString("others");
         if (value == "supported")
            return true;
         if (value == "unsupported")
            return false;

         throw shared::exception::CInvalidParameter("Error reading package.json : supported platforms is invalid : invalid value for \"others\"");
      }

      throw shared::exception::CInvalidParameter("Error reading package.json : supported platforms node is not well formed");
   }

   bool CSupportedPlatformsChecker::isVersionSupported(const shared::CDataContainer& supportedPlatformsVersionNode)
   {
      if (supportedPlatformsVersionNode.exists("from"))
      {
         shared::versioning::CVersion fromVersion(supportedPlatformsVersionNode.getString("from"));
         return COperatingSystem::getVersion() >= fromVersion;
      }

      if (supportedPlatformsVersionNode.exists("to"))
      {
         shared::versioning::CVersion toVersion(supportedPlatformsVersionNode.getString("to"));
         return COperatingSystem::getVersion() <= toVersion;
      }

      throw shared::exception::CInvalidParameter("Error reading package.json : supported platforms is invalid : invalid version description");
   }

} // namespace tools