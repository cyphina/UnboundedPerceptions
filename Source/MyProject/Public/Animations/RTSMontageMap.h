#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RTSMontageMap.generated.h"

/**
 * Holds all the possible animation montages a unit has which can be queried by tag.
 * Mostly used in the ability system which has spells which may only work with units with certain  classes of skeletons
 * of units (e.g. double strike only works with humanoids)
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced)
class MYPROJECT_API URTSMontageMap : public UPrimaryDataAsset
{
   GENERATED_BODY()

public:
   TSoftObjectPtr<UAnimMontage> GetAnimMontage(FGameplayTag tagToQuery) const;

private:

   UPROPERTY(EditAnywhere, Category = "Montage Prototypes")
   TMap<FGameplayTag, TSoftObjectPtr<UAnimMontage>> unitMontages;
};
