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

   DECLARE_CAPACITY(BlindLockingModeCapacity, "LockingMode", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CBlindLockingModeHistorizer::CBlindLockingModeHistorizer(const std::string& keywordName)
      : CSingleHistorizableData<EBlindLockingMode>(keywordName,
                                                   BlindLockingModeCapacity(),
                                                   shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet,
                                                   EBlindLockingMode::kNormal)
   {
   }

   CBlindLockingModeHistorizer::~CBlindLockingModeHistorizer()
   {
   }
} // namespace specificHistorizers
