#include "stdafx.h"
#include "FakePluginConfiguration.h"
#include <shared/Log.h>

CFakePluginConfiguration::~CFakePluginConfiguration()
{
}

void CFakePluginConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_configuration.initializeWith(data);
}

EEnumType CFakePluginConfiguration::getEnumParameter() const
{
   // Enum type, declare keys labels
   static const shared::CDataContainer::EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
      ("EnumValue1", kEnumValue1)
      ("EnumValue2", kEnumValue2)
      ("EnumValue3", kEnumValue3);

   return m_configuration.getEnumValue<EEnumType>("EnumParameter", EEnumTypeNames);
}

void CFakePluginConfiguration::trace() const
{
   try
   {
      // Get simple parameters
      YADOMS_LOG(information) << "CFakePlugin::doWork, parameter 'StringParameter' is "
         << (m_configuration.getString("StringParameter").empty() ? "empty" : m_configuration.getString("StringParameter"))
        ;
      YADOMS_LOG(information) << "CFakePlugin::doWork, parameter 'BoolParameter' is " << m_configuration.getBool("BoolParameter");
      YADOMS_LOG(information) << "CFakePlugin::doWork, parameter 'DecimalParameter' is " << m_configuration.getDouble("DecimalParameter");
      YADOMS_LOG(information) << "CFakePlugin::doWork, parameter 'IntParameter' is " << m_configuration.getInt("IntParameter");

      // Enum
      YADOMS_LOG(information) << "CFakePlugin::doWork, parameter 'EnumParameter' is " << getEnumParameter();

      // Parameters in sections
      YADOMS_LOG(information) << "CFakePlugin::doWork, parameter 'MySection.SubIntParameter' is " << m_configuration.getInt("MySection.content.SubIntParameter");
      YADOMS_LOG(information) << "CFakePlugin::doWork, parameter 'MySection.SubStringParameter' is "
         << (m_configuration.getString("MySection.content.SubStringParameter").empty() ? "empty" : m_configuration.getString("MySection.content.SubStringParameter"))
        ;
   }
   catch (const shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Parameter not found : " << e.what();
   }
   catch (const shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Parameter value out of range : " << e.what();
   }
}

