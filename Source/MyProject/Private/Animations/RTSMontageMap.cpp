#include "MyProject.h"
#include "RTSMontageMap.h"

#include "Engine/AssetManager.h"

TSoftObjectPtr<UAnimMontage> URTSMontageMap::GetAnimMontage(FGameplayTag tagToQuery) const
{
   if(unitMontages.Contains(tagToQuery))
   {
      return unitMontages[tagToQuery];
   }

   return nullptr;
}
