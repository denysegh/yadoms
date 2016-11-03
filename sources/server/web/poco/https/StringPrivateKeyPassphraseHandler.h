#pragma once

#include <Poco/Net/PrivateKeyPassphraseHandler.h>

namespace web {
   namespace poco {
      namespace https {

      class CStringPrivateKeyPassphraseHandler : public Poco::Net::PrivateKeyPassphraseHandler
      {
      public:
         CStringPrivateKeyPassphraseHandler(const std::string & passphrase);
         /// Creates the CStringPrivateKeyPassphraseHandler.

         ~CStringPrivateKeyPassphraseHandler();
         /// Destroys the CStringPrivateKeyPassphraseHandler.

         void onPrivateKeyRequested(const void* pSender, std::string& privateKey);

      private:
         std::string m_passphrase;
      };


      } //namespace https
   } //namespace poco
} //namespace web

