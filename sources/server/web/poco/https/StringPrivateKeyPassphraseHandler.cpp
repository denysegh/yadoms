#include "stdafx.h"
#include "StringPrivateKeyPassphraseHandler.h"

namespace web {
   namespace poco {
      namespace https {

      CStringPrivateKeyPassphraseHandler::CStringPrivateKeyPassphraseHandler(const std::string & passphrase) : PrivateKeyPassphraseHandler(true), m_passphrase(passphrase)
      {
      }
      
      CStringPrivateKeyPassphraseHandler::~CStringPrivateKeyPassphraseHandler()
      {
      }

      void CStringPrivateKeyPassphraseHandler::onPrivateKeyRequested(const void* pSender, std::string& privateKey)
      {
         privateKey = m_passphrase;
      }

      } //namespace https
   } //namespace poco
} //namespace web