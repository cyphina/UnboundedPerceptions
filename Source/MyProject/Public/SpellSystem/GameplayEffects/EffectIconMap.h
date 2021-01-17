#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "EffectIconMap.generated.h"

UCLASS()
class MYPROJECT_API UEffectIcons : public UDataAsset
{
   GENERATED_BODY()

public:
   UPROPERTY(EditAnywhere)
   TMap<FGameplayTag, UTexture2D*> effectIconMap;
};
