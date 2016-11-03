#pragma once

#include <Poco/Net/HTTPServer.h>

#include "web/IWebServer.h"
#include "web/rest/service/IRestService.h"

#include "HttpRequestHandlerFactory.h"
#include "WebServerConfiguration.h"

namespace web { namespace poco {

   //
   //\brief An embedded web server which supports REST api
   //
   class CWebServer : public IWebServer
   {
   public:
      //
      //\brief              Create aa embedded w<eb server
      //\param[in] webConfiguration  configuration container
      //\param[in] doc_root path to folder containing html e.g. "./"
      //\param[in] restKeywordBase the string which identifies a rest url ex: /rest/
      //
      CWebServer(CWebServerConfiguration & webConfiguration, const std::string & doc_root, const std::string & restKeywordBase, const std::string & webSocketKeywordBase);

      //
      //\brief Descturtor
      //
      virtual ~CWebServer();

      // IWebServer implementation
      virtual void start();
      virtual void stop();
      virtual IWebServerConfigurator* getConfigurator();
      // [END] IWebServer implementation

   private:
      boost::shared_ptr<Poco::Net::HTTPServer> m_embeddedWebServer;

      //Poco::SharedPtr is needed (not boost::shared_ptr)
      Poco::SharedPtr<CHttpRequestHandlerFactory> m_httpRequestHandlerFactory;
   };


} //namespace poco
} //namespace web

