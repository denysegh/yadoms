#include "stdafx.h"

#include <Poco/Net/PrivateKeyFactory.h>
#include <Poco/Net/PrivateKeyFactoryMgr.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/TemporaryFile.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/InvalidCertificateHandler.h>

#include <shared/Log.h>
#include <shared/CurrentTime/Provider.h>

#include "https/ConfigurationItems.h"
#include "https/StringPrivateKeyPassphraseHandler.h"

#include "WebServerConfiguration.h"

namespace web {
   namespace poco {

   CWebServerConfiguration::CWebServerConfiguration(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager, boost::shared_ptr<startupOptions::IStartupOptions> startupOptions)
      :  m_configurationManager(configurationManager),
         m_ip(startupOptions->getWebServerIPAddress()),
         m_useSSL(false),
         m_port(startupOptions->getWebServerPortNumber()),
         m_securedPort(startupOptions->getSSLWebServerPortNumber())
   {
      if (!configurationManager->exists(https::CConfigurationItems::Section, https::CConfigurationItems::Enabled))
      {
         //if the line desn't exists we create it as deactivated
         database::entities::CConfiguration defaultConfiguration;
         defaultConfiguration.Section = https::CConfigurationItems::Section;
         defaultConfiguration.Name = https::CConfigurationItems::Enabled;
         defaultConfiguration.Value = "false";
         defaultConfiguration.DefaultValue = "false";
         defaultConfiguration.Description = "HTTPS enabled";
         defaultConfiguration.SecurityAccess = database::entities::ESecurityAccess::kNone;
         defaultConfiguration.LastModificationDate = shared::currentTime::Provider().now();

         configurationManager->create(defaultConfiguration);
      }

      boost::shared_ptr<database::entities::CConfiguration> enabledConfiguration = configurationManager->getConfiguration(https::CConfigurationItems::Section, https::CConfigurationItems::Enabled);
      if (boost::iequals(enabledConfiguration->Value(), "true"))
      {
         //https is activated
         m_useSSL = true;
      }
   }
      
   CWebServerConfiguration::~CWebServerConfiguration()
   {
   }

   void CWebServerConfiguration::configureSSL()
   {
      //we make it only if necessary
      if (m_useSSL)
      {
         //https is activated
         m_useSSL = true;
         //Private key management
         if (!m_configurationManager->exists(https::CConfigurationItems::Section, https::CConfigurationItems::Key))
         {
            //if the line desn't exists we create it as deactivated
            database::entities::CConfiguration defaultConfiguration;
            defaultConfiguration.Section = https::CConfigurationItems::Section;
            defaultConfiguration.Name = https::CConfigurationItems::Key;
            defaultConfiguration.Value = "";
            defaultConfiguration.DefaultValue = "";
            defaultConfiguration.Description = "Private Key of the certificate";
            defaultConfiguration.SecurityAccess = database::entities::ESecurityAccess::kNone;
            defaultConfiguration.LastModificationDate = shared::currentTime::Provider().now();

            m_configurationManager->create(defaultConfiguration);
         }

         boost::shared_ptr<database::entities::CConfiguration> keyConfiguration = m_configurationManager->getConfiguration(https::CConfigurationItems::Section, https::CConfigurationItems::Key);

         //Certificate management
         if (!m_configurationManager->exists(https::CConfigurationItems::Section, https::CConfigurationItems::Certificate))
         {
            //if the line desn't exists we create it as deactivated
            database::entities::CConfiguration defaultConfiguration;
            defaultConfiguration.Section = https::CConfigurationItems::Section;
            defaultConfiguration.Name = https::CConfigurationItems::Certificate;
            defaultConfiguration.Value = "";
            defaultConfiguration.DefaultValue = "";
            defaultConfiguration.Description = "SSL Certificate content";
            defaultConfiguration.SecurityAccess = database::entities::ESecurityAccess::kNone;
            defaultConfiguration.LastModificationDate = shared::currentTime::Provider().now();

            m_configurationManager->create(defaultConfiguration);
         }

         boost::shared_ptr<database::entities::CConfiguration> certificateConfiguration = m_configurationManager->getConfiguration(https::CConfigurationItems::Section, https::CConfigurationItems::Certificate);

         std::stringstream ss(certificateConfiguration->Value());
         //we save the certificate into the KeyHandler for OpenSSL
         Poco::TemporaryFile certFile;
         certFile.keepUntilExit();
         std::ofstream ostr(certFile.path().c_str());
         ostr << keyConfiguration->Value();
         ostr << std::endl;
         ostr << certificateConfiguration->Value();
         YADOMS_LOG(debug) << "SSL Certificate file created in " << certFile.path();

         //password management
         if (!m_configurationManager->exists(https::CConfigurationItems::Section, https::CConfigurationItems::Passphrase))
         {
            //if the line desn't exists we create it as deactivated
            database::entities::CConfiguration defaultConfiguration;
            defaultConfiguration.Section = https::CConfigurationItems::Section;
            defaultConfiguration.Name = https::CConfigurationItems::Passphrase;
            defaultConfiguration.Value = "";
            defaultConfiguration.DefaultValue = "";
            defaultConfiguration.Description = "Passphrase for the private key of the certificate";
            defaultConfiguration.SecurityAccess = database::entities::ESecurityAccess::kNone;
            defaultConfiguration.LastModificationDate = shared::currentTime::Provider().now();

            m_configurationManager->create(defaultConfiguration);
         }

         boost::shared_ptr<database::entities::CConfiguration> passphraseConfiguration = m_configurationManager->getConfiguration(https::CConfigurationItems::Section, https::CConfigurationItems::Passphrase);

         //we register passphrase manager
         //POCO_REGISTER_KEYFACTORY(web::poco::https, CStringPrivateKeyPassphraseHandler);

         //we set the key in the provider, so it can provide it when SSL will ask it 
         //CStringPrivateKeyPassphraseHandler* pPassphraseHandler = dynamic_cast<CStringPrivateKeyPassphraseHandler*>(Poco::Net::SSLManager::instance().privateKeyFactoryMgr().getFactory("CStringPrivateKeyPassphraseHandler"));
         //pKeyHandler->setPassphrase(passphraseConfiguration->Value());

         //apply Open SSL configuration to SSLManager
         Poco::SharedPtr<https::CStringPrivateKeyPassphraseHandler> passphraseHandler = new https::CStringPrivateKeyPassphraseHandler(passphraseConfiguration->Value());
         Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> validationCertHandler = new Poco::Net::AcceptCertificateHandler(true);
         Poco::Net::Context::Ptr pContext = new Poco::Net::Context(Poco::Net::Context::SERVER_USE, certFile.path(), "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
         Poco::Net::SSLManager::instance().initializeServer(passphraseHandler, validationCertHandler, pContext);
      }
   }

   std::string CWebServerConfiguration::ip() const
   {
      return m_ip;
   }
   bool CWebServerConfiguration::useSSL() const
   {
      return m_useSSL;
   }

   unsigned short CWebServerConfiguration::port() const
   {
      return m_port;
   }

   unsigned short CWebServerConfiguration::securedPort() const
   {
      return m_securedPort;
   }
   
   } //namespace poco
} //namespace web