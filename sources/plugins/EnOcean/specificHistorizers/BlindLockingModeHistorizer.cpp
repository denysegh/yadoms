#include "stdafx.h"
#include "BlindLockingModeHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>


namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EBlindLockingMode,
      ((Normal))
      ((Blockage))
      ((Alarm))
   );

   CBlindLockingModeHistorizer::CBlindLockingModeHistorizer(const std::string& keywordName)
      : CSingleHistorizableData<EBlindLockingMode>(keywordName,
                                                   shared::plugin::yPluginApi::CStandardCapacity("LockingMode",
                                                                                                 shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
                                                                                                 shared::plugin::yPluginApi::EKeywordDataType::kEnum),
                                                   shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet,
                                                   EBlindLockingMode::kNormal)
   {
   }

   CBlindLockingModeHistorizer::~CBlindLockingModeHistorizer()
   {
   }
} // namespace specificHistorizers
