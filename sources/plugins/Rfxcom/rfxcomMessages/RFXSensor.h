#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The RFXSensor protocol support (reception only)
   //--------------------------------------------------------------
   class CRFXSensor : public IRfxcomMessage
   {
   public:
      CRFXSensor(boost::shared_ptr<yApi::IYPluginApi> api,
                 const RBUF& rbuf,
                 size_t rbufSize);

      virtual ~CRFXSensor();

      // IRfxcomMessage implementation
      boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      void filter() const override;
      void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      const std::string& getDeviceName() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() override;
      // [END] IRfxcomMessage implementation

   protected:
      void buildDeviceName();
      void buildDeviceModel();

      //--------------------------------------------------------------
      /// \brief	                        Process the message sub-type
      /// \param[in] msgId                Message ID
      //--------------------------------------------------------------
      static void processMessage(unsigned int msgId);

   private:
      //--------------------------------------------------------------
      /// \brief	The device sub-type
      //--------------------------------------------------------------
      unsigned char m_subType;

      //--------------------------------------------------------------
      /// \brief	The device id
      //--------------------------------------------------------------
      unsigned short m_id;

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	The device model
      //--------------------------------------------------------------
      std::string m_deviceModel;

      //--------------------------------------------------------------
      /// \brief	The temperature (°C)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CTemperature> m_temperature;

      //--------------------------------------------------------------
      /// \brief	The A/D voltage (V)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CVoltage> m_adVoltage;

      //--------------------------------------------------------------
      /// \brief	The voltage (V)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CVoltage> m_voltage;

      //--------------------------------------------------------------
      /// \brief	The signalPower (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSignalPower> m_signalPower;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages


