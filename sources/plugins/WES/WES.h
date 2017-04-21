#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "WESConfiguration.h"
#include "IOManager.h"
#include "WESFactory.h"
#include <shared/event/EventTimer.h>

#include "equipments/IEquipment.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//-----------------------------------------------------
///\brief The plugin state
//-----------------------------------------------------

enum EWESPluginState
{
   kUndefined = 0,
   kStop,
   kInitialization,
   kInitializationError,
   kReady,
   kupdateConfiguration,
   kAtLeastOneConnectionFaulty,
   kRunning
};

//--------------------------------------------------------------
/// \brief	This class is the WES plugin
//--------------------------------------------------------------
class CWES : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CWES();

   //--------------------------------------------------------------
   /// \brief	Update of the configuration
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData);

   void setPluginState(boost::shared_ptr<yApi::IYPluginApi> api, EWESPluginState newState);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CWES();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:

   //--------------------------------------------------------------
   /// \brief Configuration of the device
   //--------------------------------------------------------------
   boost::shared_ptr<CWESConfiguration> m_configuration;

   //--------------------------------------------------------------
   /// \brief The factory
   //--------------------------------------------------------------
   boost::shared_ptr<CWESFactory> m_factory;

   //--------------------------------------------------------------
   /// \brief The IO Manager
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> m_ioManager;

   //--------------------------------------------------------------
   /// \brief	Refresh timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_refreshTimer;

   //--------------------------------------------------------------
   /// \brief	PluginState
   //--------------------------------------------------------------
   EWESPluginState m_pluginState;
};