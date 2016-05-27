#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "ISecurity1Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Scurity1 X10M subtype
   //--------------------------------------------------------------
   class CSecurity1X10M : public ISecurity1Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        The RFXCom subtype value
      //--------------------------------------------------------------
      enum { rfxValue = sTypeSecX10M };

   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CSecurity1X10M();
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSecurity1X10M();
      
      // ISecurity1Subtype implementation
      std::string getModel() const override;
      void declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const override;
      void historize(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const override;
      void set(const std::string& keyword, const std::string& yadomsCommand) override;
      void reset() override;
      void setFromProtocolState(unsigned char statusByte) override;
      unsigned char toProtocolState() const override;
	  unsigned long idFromProtocol( const RBUF& rbuf ) const override;
      // [END] ISecurity1Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      yApi::historization::CSwitch m_alarm;
      yApi::historization::CSwitch m_tamper;
   };

} // namespace rfxcomMessages
