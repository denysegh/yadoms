#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include "../../IMessageHandler.h"
#include "specificHistorizers/BlindLockingModeHistorizer.h"

namespace yApi = shared::plugin::yPluginApi;

class CProfile_D2_05_Common
{
public:
   enum E_D2_05_Command
   {
      kGoToPositionAndAngle = 1,
      kStop = 2,
      kQueryPositionAndAngle = 3,
      kReplyPositionAndAngle = 4,
      kSetParameters = 5
   };

   // CMD 1 - Go to Position and Angle
   static void sendGoToPositionAndAngle(boost::shared_ptr<IMessageHandler> messageHandler,
                                        const std::string& senderId,
                                        const std::string& targetId,
                                        yApi::historization::ECurtainCommand state);
   static void sendGoToPositionAndAngle(boost::shared_ptr<IMessageHandler> messageHandler,
                                        const std::string& senderId,
                                        const std::string& targetId,
                                        unsigned int dimValue);

   // CMD 2 - Stop
   static void sendStop(boost::shared_ptr<IMessageHandler> messageHandler,
                        const std::string& senderId,
                        const std::string& targetId);

   // CMD 3 - Query Position and Angle
   static void sendQueryPositionAndAngle(boost::shared_ptr<IMessageHandler> messageHandler,
                                         const std::string& senderId,
                                         const std::string& targetId);

   // CMD 4 - Reply Position and Angle
   static std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> extractReplyPositionAndAngle(unsigned char rorg,
                                                                                                                const boost::dynamic_bitset<>& data,
                                                                                                                boost::shared_ptr<yApi::historization::CCurtain> state,
                                                                                                                boost::shared_ptr<yApi::historization::CDimmable> value,
                                                                                                                boost::shared_ptr<specificHistorizers::CBlindLockingModeHistorizer> mode);


   static void sendMessage(boost::shared_ptr<IMessageHandler> messageHandler, //TODO factoriser avec Profile_D2_01_Common.h ?
                           const std::string& senderId,
                           const std::string& targetId,
                           const boost::dynamic_bitset<>& userData,
                           const std::string& commandName);
};
