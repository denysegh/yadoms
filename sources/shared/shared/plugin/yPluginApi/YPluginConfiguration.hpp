#pragma once
#include <shared/DataContainer.h>
#include <shared/FileSystemExtension.h>


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief Manage a plugin configuration. This class is a helper
         /// to retrieve a configuration value. If value is not present in the actual
         /// configuration (probably because of plugin version was updated),
         /// this helper try to get a default value from package.json file of plugin.
         //-----------------------------------------------------
         class YPluginConfiguration
         {
         public:
            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~YPluginConfiguration()
            {
            }

            //--------------------------------------------------------------
            /// \brief		   Load configuration data
            /// \param [in] configurationData The raw configuration data (from Yadoms database)
            /// \param [in] packagePath The package path (default to "package.json" from module path)
            //--------------------------------------------------------------
            void initializeWith(const shared::CDataContainer& configurationData,
                                const boost::filesystem::path& packagePath = shared::CFileSystemExtension::getModulePath() / boost::filesystem::path("package.json"))
            {
               // Reload package file
               m_packageData.deserializeFromFile(packagePath.string());

               m_configurationData.initializeWith(configurationData);
            }

            //--------------------------------------------------------------
            /// \brief	    Check if parameter is present (only in the configuration)
            /// \param [in] parameterName    Name of the parameter
            /// \return     true if parameter found
            //--------------------------------------------------------------
            bool exists(const std::string& parameterName)
            {
               return m_configurationData.exists(parameterName);
            }

            //--------------------------------------------------------------
            /// \brief	    Get a value from configuration
            /// \param[in] parameterName Parameter to retrieve configuration
            /// \return The parameter value from configuration. If not found, search for
            /// a default value in the package.json file of plugin.
            /// \throw      shared::exception::CInvalidParameter if parameter is not found
            //--------------------------------------------------------------
            bool getBool(const std::string &parameterName) const
            {
               try
               {
                  return m_configurationData.getBool(parameterName);
               }
               catch (shared::exception::CInvalidParameter&)
               {
                  // Not found in configuration            
               }

               return m_packageData.getBool(std::string("configurationSchema.") + parameterName + (".defaultValue"));
            }

            int getInt(const std::string &parameterName) const
            {
               try
               {
                  return m_configurationData.getInt(parameterName);
               }
               catch (shared::exception::CInvalidParameter&)
               {
                  // Not found in configuration            
               }

               return m_packageData.getInt(std::string("configurationSchema.") + parameterName + (".defaultValue"));
            }
            int64_t getInt64(const std::string &parameterName) const
            {
               try
               {
                  return m_configurationData.getInt64(parameterName);
               }
               catch (shared::exception::CInvalidParameter&)
               {
                  // Not found in configuration            
               }

               return m_packageData.getInt64(std::string("configurationSchema.") + parameterName + (".defaultValue"));
            }    
            
            unsigned int getUInt(const std::string &parameterName) const
            {
               try
               {
                  return m_configurationData.getUInt(parameterName);
               }
               catch (shared::exception::CInvalidParameter&)
               {
                  // Not found in configuration            
               }

               return m_packageData.getUInt(std::string("configurationSchema.") + parameterName + (".defaultValue"));
            }        
            
            uint64_t getUInt64(const std::string &parameterName) const
            {
               try
               {
                  return m_configurationData.getUInt64(parameterName);
               }
               catch (shared::exception::CInvalidParameter&)
               {
                  // Not found in configuration            
               }

               return m_packageData.getUInt64(std::string("configurationSchema.") + parameterName + (".defaultValue"));
            }

            float getFloat(const std::string &parameterName) const
            {
               try
               {
                  return m_configurationData.getFloat(parameterName);
               }
               catch (shared::exception::CInvalidParameter&)
               {
                  // Not found in configuration            
               }

               return m_packageData.getFloat(std::string("configurationSchema.") + parameterName + (".defaultValue"));
            }

            double getDouble(const std::string &parameterName) const
            {
               try
               {
                  return m_configurationData.getDouble(parameterName);
               }
               catch (shared::exception::CInvalidParameter&)
               {
                  // Not found in configuration            
               }

               return m_packageData.getDouble(std::string("configurationSchema.") + parameterName + (".defaultValue"));
            }

            std::string getString(const std::string &parameterName) const
            {
               try
               {
                  return m_configurationData.getString(parameterName);
               }
               catch (shared::exception::CInvalidParameter&)
               {
                  // Not found in configuration            
               }

               return m_packageData.getString(std::string("configurationSchema.") + parameterName + (".defaultValue"));
            }


            //--------------------------------------------------------------
            /// \brief	    Get a enum value from configuration
            /// \param[in] parameterName Parameter to retrieve configuration
            /// \return The parameter value from configuration. If not found, search for
            /// a default value in the package.json file of plugin.
            /// \throw      shared::exception::CInvalidParameter if parameter is not found
            //--------------------------------------------------------------
            template <typename EnumType>
            EnumType getEnumValue(const std::string& parameterName,
                                  const CDataContainer::EnumValuesNames& valuesNames) const
            {
               try
               {
                  return static_cast<EnumType>(m_configurationData.getEnumValue(parameterName, valuesNames));
               }
               catch (shared::exception::CInvalidParameter&)
               {
                  // Not found in configuration            
               }

               return static_cast<EnumType>(m_packageData.getEnumValue(std::string("configurationSchema.") + parameterName + (".defaultValue"), valuesNames));
            }

         private:
            //--------------------------------------------------------------
            /// \brief	    Configuration raw data
            //--------------------------------------------------------------
            shared::CDataContainer m_configurationData;

            //--------------------------------------------------------------
            /// \brief	    Package raw data
            //--------------------------------------------------------------
            shared::CDataContainer m_packageData;
         };
      }
   }
} // namespace shared::plugin::yPluginApi	


