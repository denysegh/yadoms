#pragma once

#include <Poco/Net/PrivateKeyPassphraseHandler.h>

namespace web {
   namespace poco {
      namespace https {

      class CConfigurationItems
      {
      public:

         /// Destroys the CConfigurationItems.
         ~CConfigurationItems();

         static const std::string Section;
         static const std::string Enabled;
         static const std::string Key;
         static const std::string Certificate;
         static const std::string Passphrase;
         
      private:
         /// Creates the CConfigurationItems.
         CConfigurationItems();
      };


      } //namespace https
   } //namespace poco
} //namespace web

