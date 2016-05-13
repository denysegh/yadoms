#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "../ISIConfiguration.h"


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Class where all Keywords are created and updated
/// \note   
//--------------------------------------------------------------
class CSystemFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] api                 yPluginApi API
   /// \param[in] device              The device
   /// \param[in] configuration       Configuration
   //--------------------------------------------------------------
   CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> api, const std::string & device,const ISIConfiguration& configuration);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSystemFactory();

   void OnSpeedUpdate ( boost::shared_ptr<yApi::IYPluginApi> api , const ISIConfiguration& configuration);
   void OnSlowUpdate  ( boost::shared_ptr<yApi::IYPluginApi> api , const ISIConfiguration& configuration);
   void OnConfigurationUpdate ( boost::shared_ptr<yApi::IYPluginApi> api, const ISIConfiguration& configuration );

private:
   //--------------------------------------------------------------
   /// \brief	    Plugin name
   //--------------------------------------------------------------
   std::string m_PluginName;
};

