#include "stdafx.h"
#include "WebServer.h"
#include <shared/Log.h>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SecureServerSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/Net/NetException.h>
#include "MimeType.h"
#include <shared/ServiceLocator.h>
#include "server/IRunningInformation.h"


namespace web { namespace poco {

   CWebServer::CWebServer(CWebServerConfiguration & webConfiguration, const std::string & doc_root, const std::string & restKeywordBase, const std::string & webSocketKeywordBase)
      :m_httpRequestHandlerFactory(new CHttpRequestHandlerFactory())
   {
      //configure the factory
      m_httpRequestHandlerFactory->websiteHandlerConfigure(doc_root);
      m_httpRequestHandlerFactory->restHandlerConfigure(restKeywordBase);
      m_httpRequestHandlerFactory->webSocketConfigure(webSocketKeywordBase);

      //setup HTTPServer Params (define name and version; to match common http server configuration)
      Poco::Net::HTTPServerParams::Ptr serverParams(new Poco::Net::HTTPServerParams());
      serverParams->setServerName("Yadoms");
      boost::shared_ptr<IRunningInformation> runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
      serverParams->setSoftwareVersion(runningInformation->getSoftwareVersion().getVersion().toString(4));
      serverParams->setKeepAlive(false); //this line fix global catch exception on multiple browser refresh
      
      //in case of "0.0.0.0" or empty , then do not use it, just use port, listen on all interfaces
      Poco::Net::IPAddress address;
      if (webConfiguration.ip() == "0.0.0.0" || webConfiguration.ip().empty())
      {
         address = Poco::Net::IPAddress::wildcard();
      }
      else
      {
         if (!Poco::Net::IPAddress::tryParse(webConfiguration.ip(), address))
         {
            //invalid IP provided
            YADOMS_LOG(warning) << "*******************************************************";
            YADOMS_LOG(warning) << "Invalid IP address provided in web server configuration";
            YADOMS_LOG(warning) << "Listening on any ip address";
            YADOMS_LOG(warning) << "*******************************************************";
            address = Poco::Net::IPAddress::wildcard();
         }
      }

      // set-up a HTTPServer instance
      Poco::Net::ServerSocket svs(Poco::Net::SocketAddress(address, webConfiguration.port()));
      if (webConfiguration.useSSL())
      {
         bool fail = false;
         try {
            webConfiguration.configureSSL();
            svs = Poco::Net::SecureServerSocket(webConfiguration.securedPort());
         }
         catch (Poco::Exception & ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to configure HTTPS: " << ex.message();
            fail = true;
         }
         catch (std::exception & ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to configure HTTPS: " << ex.what();
            fail = true;
         }
         catch (...)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to configure HTTPS";
            fail = true;
         }
         if (fail)
         {
            //we activate the default classic web server
            YADOMS_LOG(warning) << "**********************************************************";
            YADOMS_LOG(warning) << "Using HTTP instead of HTTPS due to malformed configuration";
            YADOMS_LOG(warning) << "**********************************************************";
            svs = Poco::Net::ServerSocket(webConfiguration.port());
         }
      }

      bool a = false;
      int b = 0;
      svs.getLinger(a, b);
      svs.setLinger(true, 2);
      m_embeddedWebServer = boost::make_shared<Poco::Net::HTTPServer>(m_httpRequestHandlerFactory, svs, serverParams);
   }

   CWebServer::~CWebServer()
   {
      stop();
   }


   // IWebServer implementation
   void CWebServer::start()
   {
      // start the HTTPServer
      m_embeddedWebServer->start();
   }

   void CWebServer::stop()
   {
      if(m_embeddedWebServer.get())
         m_embeddedWebServer->stopAll(true);
   }

   IWebServerConfigurator* CWebServer::getConfigurator()
   {
      return m_httpRequestHandlerFactory.get();
   }




} //namespace poco
} //namespace web

