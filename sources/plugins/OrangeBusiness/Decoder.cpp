#include "stdafx.h"
#include "Decoder.h"
#include <shared/Log.h>
#include "DefaultEquipment.h"
#include "UnauthorizedException.hpp"

CDecoder::CDecoder()
{}

CDecoder::~CDecoder()
{}

std::map<std::string, boost::shared_ptr<equipments::IEquipment>> CDecoder::decodeDevicesMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                    shared::CDataContainer& message)
{
	std::map<std::string, boost::shared_ptr<equipments::IEquipment>> equipmentList;
   message.printToLog(YADOMS_LOG(trace));

   auto errorcode = message.getStringWithDefault("code","");

   if (errorcode == "UNAUTHORIZED")
      throw CUnauthorizedException(message.getString("message"));

   if (errorcode != "")
      throw shared::exception::CException(message.getString("message"));

   std::vector<shared::CDataContainer> equipments;
   message.getChilds("data", equipments);

   std::vector<shared::CDataContainer>::iterator equipmentIterator;

   for (equipmentIterator = equipments.begin(); equipmentIterator != equipments.end(); ++equipmentIterator)
   {
      std::string name = (*equipmentIterator).getString("name");
      std::string devEUI = (*equipmentIterator).getString("devEUI");
      boost::shared_ptr<equipments::CDefaultEquipment> newEquipment(boost::make_shared<equipments::CDefaultEquipment>(name, devEUI, api));
	   equipmentList.insert(std::pair<std::string, boost::shared_ptr<equipments::IEquipment>>(name, newEquipment));
      YADOMS_LOG(information) << "create device name = " << name << " devEUI = " << devEUI;
   }

   return equipmentList;
}

bool CDecoder::isFrameComplete(shared::CDataContainer& message)
{
   int page = message.getInt("page");
   int pageSize = message.getInt("size");

   if (message.getInt("totalCount") > (page * pageSize))
      return true;
   else
      return false;
}

shared::CDataContainer CDecoder::getLastData(shared::CDataContainer& response)
{
   std::vector<shared::CDataContainer> messages;
   response.getChilds("", messages);
   shared::CDataContainer lastData;

   if (messages.size() > 0)
   {
      // first message is the last message received
      auto message = messages[0];

      //Copy to the answer needed information
      lastData.set("id", message.getString("id"));
      lastData.set("timestamp", message.getString("timestamp"));
      lastData.set("payload", message.getString("value.payload"));
      lastData.set("rssi", message.getDouble("metadata.network.lora.rssi"));
      lastData.set("snr", message.getDouble("metadata.network.lora.snr"));
      lastData.set("signalLevel", message.getInt("value.signalLevel"));
   }
   return lastData;
}