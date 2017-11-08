#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace specificHistorizers
{
   DECLARE_ENUM_HEADER(EBlindLockingMode,
      ((Normal))
      ((Blockage))
      ((Alarm))
   );

   class CBlindLockingModeHistorizer : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EBlindLockingMode>
   {
   public:
      explicit CBlindLockingModeHistorizer(const std::string& keywordName);
      virtual ~CBlindLockingModeHistorizer();
   };
} // namespace specificHistorizers
