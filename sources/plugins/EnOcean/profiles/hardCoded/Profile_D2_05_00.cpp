#include "stdafx.h"
#include "Profile_D2_05_00.h"
#include "Profile_D2_05_Common.h"

CProfile_D2_05_00::CProfile_D2_05_00(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_state(boost::make_shared<yApi::historization::CCurtain>("state")),
     m_value(boost::make_shared<yApi::historization::CDimmable>("value")),
     m_mode(boost::make_shared<specificHistorizers::CBlindLockingModeHistorizer>("mode")),
     m_historizers({m_state, m_value, m_mode})
{
}

CProfile_D2_05_00::~CProfile_D2_05_00()
{
}

const std::string& CProfile_D2_05_00::profile() const
{
   static const std::string profile("D2-05-00");
   return profile;
}

const std::string& CProfile_D2_05_00::title() const
{
   static const std::string title("Blinds Control for Position and Angle");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_05_00::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_05_00::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   CProfile_D2_05_Common::sendQueryPositionAndAngle(messageHandler,
                                                    senderId,
                                                    m_deviceId);
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_05_00::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   return CProfile_D2_05_Common::extractReplyPositionAndAngle(rorg,
                                                              data,
                                                              m_state,
                                                              m_value,
                                                              m_mode);
}

void CProfile_D2_05_00::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   if (keyword == m_state->getKeyword())
   {
      m_state->setCommand(commandBody);

      switch (m_state->get())
      {
      case yApi::historization::ECurtainCommand::kOpenValue:
      case yApi::historization::ECurtainCommand::kCloseValue:
         CProfile_D2_05_Common::sendGoToPositionAndAngle(messageHandler,
                                                         senderId,
                                                         m_deviceId,
                                                         m_state->get());
         break;
      case yApi::historization::ECurtainCommand::kStopValue:
         CProfile_D2_05_Common::sendStop(messageHandler,
                                         senderId,
                                         m_deviceId);
         break;
      default:
         throw std::invalid_argument((boost::format("Unsupported curtain state %1%") % m_state->get()).str());
      }
   }
   else if (keyword == m_value->getKeyword())
   {
      m_value->setCommand(commandBody);

      CProfile_D2_05_Common::sendGoToPositionAndAngle(messageHandler,
                                                      senderId,
                                                      m_deviceId,
                                                      m_value->get());
   }
   else if (keyword == m_mode->getKeyword())
   {
      m_mode->setCommand(commandBody);

      CProfile_D2_05_Common::sendGoToPositionAndAngle(messageHandler,
                                                      senderId,
                                                      m_deviceId,
                                                      m_mode->get());
   }
}

void CProfile_D2_05_00::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   enum
   {
      kDontChangeMeasureDurationOfVerticalRun = 32767,
      kDontChangeMeasureDurationOfRotation = 255
   };
   auto measuredDurationOfVerticalRunValue = deviceConfiguration.get<bool>("measuredDurationOfVerticalRun.checkbox")
                                                ? static_cast<unsigned int>(deviceConfiguration.get<double>("measuredDurationOfVerticalRun.content.value") * 100)
                                                : kDontChangeMeasureDurationOfVerticalRun;
   auto measuredDurationOfRotationValue = deviceConfiguration.get<bool>("measuredDurationOfRotation.checkbox")
                                             ? static_cast<unsigned int>(deviceConfiguration.get<double>("measuredDurationOfRotation.content.value") * 100)
                                             : kDontChangeMeasureDurationOfRotation;
   auto alarmActionValue = deviceConfiguration.get<std::string>("alarmAction");
   enum EAlarmAction
   {
      kNoAction = 0,
      kImmediateStop = 1,
      kGoUp = 2,
      kGoDown = 3,
      kNoChange = 7
   };
   EAlarmAction alarmAction;
   if (alarmActionValue == "noAction")
      alarmAction = kNoAction;
   else if (alarmActionValue == "immediateStop")
      alarmAction = kImmediateStop;
   else if (alarmActionValue == "goUp")
      alarmAction = kGoUp;
   else if (alarmActionValue == "goDown")
      alarmAction = kGoDown;
   else
      alarmAction = kNoChange;

   CProfile_D2_05_Common::sendSetParameters(messageHandler,
                                            senderId,
                                            m_deviceId,
                                            measuredDurationOfVerticalRunValue,
                                            measuredDurationOfRotationValue,
                                            alarmAction);
}
