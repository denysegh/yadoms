#include "stdafx.h"
#include "VLD_SendMessage.h"
#include <shared/Log.h>
#include "RadioErp1SendMessage.h"
#include "profiles/bitsetHelpers.hpp"
#include "ResponseReceivedMessage.h"


namespace message
{
   void CVLD_message::send(boost::shared_ptr<IMessageHandler> messageHandler,
                           const std::string& senderId,
                           const std::string& targetId,
                           const boost::dynamic_bitset<>& userData,
                           const std::string& commandName)
   {
      CRadioErp1SendMessage command(CRorgs::kVLD_Telegram,
                                    senderId,
                                    targetId,
                                    0);

      command.userData(bitset_to_bytes(userData));

      boost::shared_ptr<const CEsp3ReceivedPacket> answer;
      if (!messageHandler->send(command,
                                [](boost::shared_ptr<const CEsp3ReceivedPacket> esp3Packet)
                             {
                                return esp3Packet->header().packetType() == RESPONSE;
                             },
                                [&](boost::shared_ptr<const CEsp3ReceivedPacket> esp3Packet)
                             {
                                answer = esp3Packet;
                             }))
      YADOMS_LOG(error) << "Fail to send message to " << targetId << " : no answer to \"" << commandName << "\"";

      auto response = boost::make_shared<CResponseReceivedMessage>(answer);

      if (response->returnCode() != CResponseReceivedMessage::RET_OK)
      YADOMS_LOG(error) << "Fail to send message to " << targetId << " : \"" << commandName << "\" returns " << response->returnCode();
   }
}
