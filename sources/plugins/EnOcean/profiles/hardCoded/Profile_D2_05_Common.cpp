#include "stdafx.h"
#include "Profile_D2_05_Common.h"
#include "../bitsetHelpers.hpp"
#include "../../message/RadioErp1SendMessage.h"
#include <shared/Log.h>
#include "Profile_D2_01_Common.h"
#include "message/VLD_SendMessage.h"


void CProfile_D2_05_Common::sendGoToPositionAndAngle(boost::shared_ptr<IMessageHandler> messageHandler,
                                                     const std::string& senderId,
                                                     const std::string& targetId,
                                                     yApi::historization::ECurtainCommand state)
{
   unsigned int value;
   switch (state)
   {
   case yApi::historization::ECurtainCommand::kCloseValue:
      value = 0;
      break;
   case yApi::historization::ECurtainCommand::kOpenValue:
      value = 100;
      break;
   default:
      throw std::invalid_argument((boost::format("Unsupported curtain state %1%") % state).str());
   }

   sendGoToPositionAndAngle(messageHandler,
                            senderId,
                            targetId,
                            value);
}

void CProfile_D2_05_Common::sendGoToPositionAndAngle(boost::shared_ptr<IMessageHandler> messageHandler,
                                                     const std::string& senderId,
                                                     const std::string& targetId,
                                                     unsigned int dimValue)
{
   boost::dynamic_bitset<> userData(4 * 8);
   bitset_insert(userData, 1, 7, dimValue);
   bitset_insert(userData, 9, 7, dimValue);
   bitset_insert(userData, 17, 3, 0); // Repositioning : Go directly to POS/ANG
   bitset_insert(userData, 21, 3, 0); // Locking modes : Do not change
   bitset_insert(userData, 24, 4, 0); // Channel : Channel 1
   bitset_insert(userData, 28, 4, kGoToPositionAndAngle);

   message::CVLD_message::send(messageHandler,
                               senderId,
                               targetId,
                               userData,
                               "Go to Position and Angle");
}

void CProfile_D2_05_Common::sendGoToPositionAndAngle(boost::shared_ptr<IMessageHandler> messageHandler,
                                                     const std::string& senderId,
                                                     const std::string& targetId,
                                                     specificHistorizers::EBlindLockingMode mode)
{
   enum
   {
      kDontChangePosOrAngle = 127
   };

   boost::dynamic_bitset<> userData(4 * 8);
   bitset_insert(userData, 1, 7, kDontChangePosOrAngle);
   bitset_insert(userData, 9, 7, kDontChangePosOrAngle);
   bitset_insert(userData, 17, 3, 0); // Repositioning : Go directly to POS/ANG
   bitset_insert(userData, 21, 3, mode); // Locking modes : Do not change
   bitset_insert(userData, 24, 4, 0); // Channel : Channel 1
   bitset_insert(userData, 28, 4, kGoToPositionAndAngle);

   message::CVLD_message::send(messageHandler,
                               senderId,
                               targetId,
                               userData,
                               "Go to Position and Angle (set mode)");
}

void CProfile_D2_05_Common::sendStop(boost::shared_ptr<IMessageHandler> messageHandler,
                                     const std::string& senderId,
                                     const std::string& targetId)
{
   boost::dynamic_bitset<> userData(1 * 8);
   bitset_insert(userData, 0, 4, 0); // Channel : Channel 1
   bitset_insert(userData, 4, 4, kStop);

   message::CVLD_message::send(messageHandler,
                               senderId,
                               targetId,
                               userData,
                               "Stop");
}

void CProfile_D2_05_Common::sendQueryPositionAndAngle(boost::shared_ptr<IMessageHandler> messageHandler,
                                                      const std::string& senderId,
                                                      const std::string& targetId)
{
   boost::dynamic_bitset<> userData(1 * 8);
   bitset_insert(userData, 0, 4, 0); // Channel : Channel 1
   bitset_insert(userData, 4, 4, kQueryPositionAndAngle);

   message::CVLD_message::send(messageHandler,
                               senderId,
                               targetId,
                               userData,
                               "Query Position and Angle");
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_05_Common::extractReplyPositionAndAngle(unsigned char rorg,
                                                                                                                             const boost::dynamic_bitset<>& data,
                                                                                                                             boost::shared_ptr<yApi::historization::CCurtain> state,
                                                                                                                             boost::shared_ptr<yApi::historization::CDimmable> value,
                                                                                                                             boost::shared_ptr<specificHistorizers::CBlindLockingModeHistorizer> mode)
{
   // Some devices supports several RORG telegrams, ignore non-VLD telegrams
   if (rorg != CRorgs::ERorgIds::kVLD_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   if (bitset_extract(data, 28, 4) != kReplyPositionAndAngle)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   // Return only the concerned historizers
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   auto ioChannel = bitset_extract(data, 24, 4);
   int posValue = bitset_extract(data, 1, 7);
   int angValue = bitset_extract(data, 9, 7);
   auto lockingMode = bitset_extract(data, 21, 3);

   if (ioChannel != 0)
   YADOMS_LOG(warning) << "ReplyPositionAndAngle : received unsupported ioChannel value " << ioChannel;

   enum
   {
      kPosOrAngleUnknown = 127
   };

   if (posValue == kPosOrAngleUnknown)
   {
      if (angValue == kPosOrAngleUnknown)
         return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

      if (!!value)
      {
         value->set(angValue);
         historizers.push_back(value);
      }
      if (!!state)
      {
         state->set(angValue > 49 ? yApi::historization::ECurtainCommand::kOpen : yApi::historization::ECurtainCommand::kClose);
         historizers.push_back(value);
      }
   }
   else
   {
      if (!!value)
      {
         value->set(posValue);
         historizers.push_back(value);
      }
      if (!!state)
      {
         state->set(posValue > 49 ? yApi::historization::ECurtainCommand::kOpen : yApi::historization::ECurtainCommand::kClose);
         historizers.push_back(value);
      }
   }

   if (!!mode)
   {
      enum ELockingMode
      {
         kNormal = 0,
         kBlockage = 1,
         kAlarm = 2
      };

      switch (lockingMode)
      {
      case kNormal:
         mode->set(specificHistorizers::EBlindLockingMode::kNormal);
         historizers.push_back(mode);
         break;
      case kBlockage:
         mode->set(specificHistorizers::EBlindLockingMode::kBlockage);
         historizers.push_back(mode);
         break;
      case kAlarm:
         mode->set(specificHistorizers::EBlindLockingMode::kAlarm);
         historizers.push_back(mode);
         break;
      default:
         YADOMS_LOG(error) << "Unsupported received locking mode";
         break;
      }
   }

   return historizers;
}

void CProfile_D2_05_Common::sendSetParameters(boost::shared_ptr<IMessageHandler> messageHandler,
                                              const std::string& senderId,
                                              const std::string& targetId,
                                              unsigned int measuredDurationOfVerticalRunMs,
                                              unsigned int measuredDurationOfRotationMs,
                                              unsigned int alarmActionValue)
{
   boost::dynamic_bitset<> userData(5 * 8);
   bitset_insert(userData, 1, 15, measuredDurationOfVerticalRunMs);
   bitset_insert(userData, 16, 8, measuredDurationOfRotationMs);
   bitset_insert(userData, 29, 3, alarmActionValue);
   bitset_insert(userData, 32, 4, 0); // Channel : Channel 1
   bitset_insert(userData, 36, 4, kSetParameters);

   message::CVLD_message::send(messageHandler,
                               senderId,
                               targetId,
                               userData,
                               "Set Parameters");
}
