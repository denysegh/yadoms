#include "ConfigurationItems.h"

namespace web {
   namespace poco {
      namespace https {

         CConfigurationItems::CConfigurationItems()
         {
         }
         
         CConfigurationItems::~CConfigurationItems()
         {
         }

         const std::string CConfigurationItems::Section = "https";
         const std::string CConfigurationItems::Enabled = "enabled";
         const std::string CConfigurationItems::Key = "key";
         const std::string CConfigurationItems::Certificate = "certificate";
         const std::string CConfigurationItems::Passphrase = "passphrase";
         
      } //namespace https
   } //namespace poco
} //namespace web