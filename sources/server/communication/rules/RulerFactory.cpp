#include "stdafx.h"
#include "RulerFactory.h"
#include "standard/DeviceManager.h"
#include "rfxLanXpl/DeviceManager.h"
#include <shared/exception/Exception.hpp>
#include "ICommandRule.h"

namespace communication { namespace rules {

   CRulerFactory::CRulerFactory()
      :m_standardFactories(boost::shared_ptr<IDeviceManager>(new standard::CDeviceManager()))
   {
      //all specific rules factory
      m_allSpecificRuleFactories.push_back(boost::shared_ptr<IDeviceManager>(new rfxLanXpl::CDeviceManager()));
   }

   CRulerFactory::~CRulerFactory()
   {
   }

   boost::shared_ptr<IRule> CRulerFactory::identifyRule(shared::xpl::CXplMessage & msg)
   {
      BOOST_FOREACH(boost::shared_ptr<IDeviceManager> currentFactory, m_allSpecificRuleFactories)
      {
         if(currentFactory->isHandled(msg))
         {
            boost::shared_ptr<IRule> foundRule = currentFactory->identifyRule(msg, m_instanceManager);
            if(foundRule.get() != NULL)
               return foundRule;
         }
      }
      return m_standardFactories->identifyRule(msg, m_instanceManager);
   }

   boost::shared_ptr<IRule> CRulerFactory::identifyRule(database::entities::CDevice & device)
   {
      BOOST_FOREACH(boost::shared_ptr<IDeviceManager> currentFactory, m_allSpecificRuleFactories)
      {
         if(currentFactory->isHandled(device))
         {
            boost::shared_ptr<IRule> foundRule = currentFactory->identifyRule(device, m_instanceManager);
            if(foundRule.get() != NULL)
               return foundRule;
         }
      }
      return m_standardFactories->identifyRule(device, m_instanceManager);
   }



   boost::shared_ptr< shared::xpl::CXplMessage > CRulerFactory::createXplCommand(database::entities::CDevice & targetDevice, communication::command::CDeviceCommand & deviceCommand)
   {
      //identify the device Rule 
      boost::shared_ptr<rules::IRule> rule = identifyRule(targetDevice);

      if(rule)
      {
         //check if the rule handled commands
         boost::shared_ptr<ICommandRule> commandRule = boost::dynamic_pointer_cast<ICommandRule>(rule);

         if(commandRule)
         {
            return commandRule->createXplCommand(targetDevice, deviceCommand);
         }
         else
         {
            std::string errorMessage = (boost::format("The device (id=%1% name=%2%) do not support commands") % targetDevice.Id() % targetDevice.Name()).str();
            throw shared::exception::CException(errorMessage);
         }
      }
      else
      {
         std::string errorMessage = (boost::format("The device (id=%1% name=%2%) is not handled by Yadoms") % targetDevice.Id() % targetDevice.Name()).str();
         throw shared::exception::CException(errorMessage);
      }
   }


   std::vector<std::string> CRulerFactory::getHardwareProtocols(const std::string & hardwareIdentifier)
   {
      BOOST_FOREACH(boost::shared_ptr<IDeviceManager> currentFactory, m_allSpecificRuleFactories)
      {
         if(currentFactory->matchHardware(hardwareIdentifier))
         {
            return currentFactory->getHandledProtocols();
         }
      }
      return m_standardFactories->getHandledProtocols();
   }

   std::string CRulerFactory::generateVirtualDeviceIdentifier(const std::string & hardwareIdentifier, const std::string & protocolIdentifier)
   {
      BOOST_FOREACH(boost::shared_ptr<IDeviceManager> currentFactory, m_allSpecificRuleFactories)
      {
         if(currentFactory->matchHardware(hardwareIdentifier))
         {
            return currentFactory->generateVirtualDeviceIdentifier(protocolIdentifier, m_instanceManager);
         }
      }
      return m_standardFactories->generateVirtualDeviceIdentifier(protocolIdentifier, m_instanceManager);
   }


} //namespace rules
} //namespace communication

