// Created 1/3/21 3:47 PM

#pragma once
#include "GameInstanceSubsystem.h"
#include "GlobalDataAssetsSS.generated.h"

/**
 * @brief Store references to global resources that live throughout the game's execution
 */
UCLASS()
class MYPROJECT_API UGlobalDataAssetsSS : public UGameInstanceSubsystem
{
   GENERATED_BODY()

 public:
   UPROPERTY(EditDefaultsOnly)
   class UDA_DefaultOffensiveTargetingScheme* defaultOffensiveTargetSchemes;

   UPROPERTY(EditDefaultsOnly)
   class UDA_DefaultSupportTargetingScheme* defaultSupportTargetSchemes;

   UPROPERTY(EditDefaultsOnly)
   class UDA_DefaultPurgeTargetingScheme* defaultPurgeTargetSchemes;

   UPROPERTY(EditDefaultsOnly)
   class UDA_DefaultDisableTargetingScheme* defaultDisableTargetSchemes;
};
