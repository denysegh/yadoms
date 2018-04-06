#include "stdafx.h"
#include "ConfigurationManager.h"
#include <shared/exception/EmptyResult.hpp>

namespace dataAccessLayer
{
   class CDefaultServerConfiguration
   {
   public:
      boost::shared_ptr<shared::CDataContainer> get() const
      {
         return m_configuration;
      }

      CDefaultServerConfiguration()
         : m_configuration(boost::make_shared<shared::CDataContainer>())
      {
         m_configuration->set("firstStart", true);

         // Accepted values for location status :
         //   - "defaut" : when location comes from the default configuration (this one)
         //   - "autoDetected" : when location was automaticaly detected (ie from IP)
         //   - "userDefined" : when location was defined by user
         m_configuration->set("location.status", "default");
         m_configuration->set("location.latitude", "48.853");
         m_configuration->set("location.longitude", "2.35");
         m_configuration->set("location.altitude", "0.0");
         m_configuration->set("location.timezone", "Europe/Paris");

         m_configuration->set("basicAuthentication.active", false);
         m_configuration->set("basicAuthentication.user", "admin");
         m_configuration->set("basicAuthentication.password", std::string());
      }

      virtual ~CDefaultServerConfiguration()
      {
      }

   private:
      const boost::shared_ptr<shared::CDataContainer> m_configuration;
   };

   CConfigurationManager::CConfigurationManager(boost::shared_ptr<database::IConfigurationRequester> configurationRequester)
      : m_configurationRequester(configurationRequester),
        m_defaultServerConfiguration(CDefaultServerConfiguration().get())
   {
   }

   CConfigurationManager::~CConfigurationManager()
   {
   }

   std::string CConfigurationManager::getExternalConfiguration(const std::string& section) const
   {
      return getConfiguration("external." + section);
   }

   void CConfigurationManager::saveExternalConfiguration(const std::string& section,
                                                         const shared::CDataContainer& value)
   {
      saveConfiguration("external." + section,
                        value);
   }

   void CConfigurationManager::notifyServerConfigurationChanged(boost::shared_ptr<const shared::CDataContainer> serverConfiguration)
   {
      for (const auto& fct : m_onServerConfigurationChangedObservers)
         fct(serverConfiguration);
   }

   boost::shared_ptr<const shared::CDataContainer> CConfigurationManager::getServerConfiguration() const
   {
      try
      {
         // Returned configuration is the default one, overriden by stored values
         auto configuration = boost::make_shared<shared::CDataContainer>(*m_defaultServerConfiguration);
         configuration->mergeFrom(shared::CDataContainer(getConfiguration("server")));
         return configuration;
      }
      catch (shared::exception::CEmptyResult&)
      {
         return m_defaultServerConfiguration;
      }
   }

   void CConfigurationManager::saveServerConfiguration(const shared::CDataContainer& newConfiguration)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_configurationMutex);

      // If location changed, mark it as user-modified
      auto currentConfiguration(*getServerConfiguration());
      auto configurationToSave(newConfiguration);
      if(newConfiguration.getString("location.latitude") != currentConfiguration.getString("location.latitude") ||
         newConfiguration.getString("location.longitude")!= currentConfiguration.getString("location.longitude") ||
         newConfiguration.getString("location.altitude") != currentConfiguration.getString("location.altitude") ||
         newConfiguration.getString("location.timezone") != currentConfiguration.getString("location.timezone"))
      {
         configurationToSave.set("location.status", "userDefined");
      }

      saveConfiguration("server", configurationToSave);
      notifyServerConfigurationChanged(getServerConfiguration());
   }

   void CConfigurationManager::resetServerConfiguration()
   {
      auto resetConfiguration = *m_defaultServerConfiguration;
      // Reset configuration must not overwrite firstStart flag
      resetConfiguration.set("firstStart", getServerConfiguration()->getBool("firstStart"));
      saveServerConfiguration(resetConfiguration);
   }

   std::string CConfigurationManager::getDatabaseVersion() const
   {
      return getConfiguration("databaseVersion");
   }

   void CConfigurationManager::subscribeOnServerConfigurationChanged(
      boost::function1<void, boost::shared_ptr<const shared::CDataContainer>> onServerConfigurationChangedFct)
   {
      m_onServerConfigurationChangedObservers.push_back(onServerConfigurationChangedFct);
   }

   shared::CDataContainer CConfigurationManager::getLocation() const
   {
      return getServerConfiguration()->getChild("location");
   }

   void CConfigurationManager::saveAutoDetectedLocation(const shared::CDataContainer& newLocation)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_configurationMutex);
      auto serverConfiguration = *getServerConfiguration();

      auto location = getLocation();

      // Overwrite location only if not set by user
      if (location.getString("status") == "userDefined")
         return;
      
      location.set("status", "autoDetected");
      location.set("latitude", newLocation.getString("latitude"));
      location.set("longitude",newLocation.getString("longitude"));
      location.set("altitude", newLocation.getString("altitude"));
      location.set("timezone", newLocation.getString("timezone"));

      serverConfiguration.set("location", location);
      saveConfiguration("server", serverConfiguration);
      notifyServerConfigurationChanged(getServerConfiguration());
   }

   shared::CDataContainer CConfigurationManager::getBasicAuthentication() const
   {
      return getServerConfiguration()->getChild("basicAuthentication");
   }

   std::string CConfigurationManager::getConfiguration(const std::string& section) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_configurationMutex);
      return m_configurationRequester->getConfiguration(section)->Value();
   }

   void CConfigurationManager::saveConfiguration(const std::string& section,
                                                 const shared::CDataContainer& value) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_configurationMutex);
      m_configurationRequester->updateConfiguration(section,
                                                    value.serialize());
   }
} //namespace dataAccessLayer 
