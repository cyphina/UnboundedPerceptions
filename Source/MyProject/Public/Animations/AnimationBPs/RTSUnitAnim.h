#pragma once
#include "AttackAnim.h"
#include "Animation/AnimInstance.h"
#include "RTSMontageMap.h"
#include "RTSUnitAnim.generated.h"

class AUnit;

/**
 * Base class for all Unit animation blueprints for Up.
 * PS: In case you want to handle animation montages using C++, inside the AnimInstance C++ class you have the ability to call functions like
 * Montage_Play / Montage_IsPlaying / Montage_Stop and provide a UAnimMontage* parameter. The character class exposes this API too.
 * We can expose notify functions (as UFUNCTIONs()) to call here as well when the notifies actually get hit in UE4.
 */
UCLASS()
class MYPROJECT_API URTSUnitAnim : public UAnimInstance
{
   GENERATED_BODY()

public:
   FOnHitNotify& OnHitNotify() { return OnHitNotifyEvent; }

   UFUNCTION(BlueprintCallable, BlueprintPure)
   TSoftObjectPtr<UAnimMontage> GetAnimMontage(FGameplayTag tagToQuery) const { return montageMap.GetDefaultObject()->GetAnimMontage(tagToQuery); }

protected:
   void AnimNotify_OnAttack(UAnimNotify* notify);

   /** True means we're currently in the air or falling */
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   bool bIsFalling;

   /** Holds current movement speed of our character */
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   float movementSpeed;

   /** Allows us to check if we have a valid montage for the scenario associated with a gameplay tag (usually a spell name) */
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
   TSubclassOf<URTSMontageMap> montageMap;

   void UpdateAnimationProperties();
   void NativeBeginPlay() override final;
   void NativeUpdateAnimation(float deltaSeconds) override final;

private:
   UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
   AUnit* unitRef;

   FOnHitNotify OnHitNotifyEvent;
};
