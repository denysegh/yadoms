#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IMessageHandler.h"

namespace yApi = shared::plugin::yPluginApi;

namespace message
{
   class CVLD_message
   {
   public:
      static void send(boost::shared_ptr<IMessageHandler> messageHandler,
                       const std::string& senderId,
                       const std::string& targetId,
                       const boost::dynamic_bitset<>& userData,
                       const std::string& commandName);
   };
}
