#pragma once

#include "dataAccessLayer/IConfigurationManager.h"
#include "startupOptions/IStartupOptions.h"

namespace web {
   namespace poco {

   class CWebServerConfiguration
   {
   public:
      //
      //\brief              Create a container that embeed web server configuration. Initialize SSL engine from database if activated
      //\param[in] configurationManager  database configuration accessor
      //\param[in] startupOptions   options provided by ini file
      //
      CWebServerConfiguration(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager, boost::shared_ptr<startupOptions::IStartupOptions> startupOptions);

      ~CWebServerConfiguration();

      std::string ip() const;
      bool useSSL() const;
      unsigned short port() const;
      unsigned short securedPort() const;

      //
      // \brief Configure the SSL part with OpenSSL
      void configureSSL();

   private:
      boost::shared_ptr<dataAccessLayer::IConfigurationManager> m_configurationManager;
      std::string m_ip;
      bool m_useSSL;
      unsigned short m_port;
      unsigned short m_securedPort;
      
   };

   } //namespace poco
} //namespace web

