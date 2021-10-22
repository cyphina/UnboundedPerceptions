// Created 6/14/21 12:57 PM

#pragma once
#include "RTSGameMode.h"
#include "RTSTrigger.generated.h"

class URTSConditional;

DECLARE_LOG_CATEGORY_EXTERN(Up_Log_Triggers, Log, All);


UCLASS(Abstract, BlueprintType, DefaultToInstanced, EditInlineNew)
class MYPROJECT_API URTSTriggerBase : public UObject
{
   GENERATED_BODY()

 public:
   URTSTriggerBase();

   UFUNCTION(BlueprintNativeEvent)
   void         TriggerEvent();
   virtual void TriggerEvent_Implementation() PURE_VIRTUAL(URTSTriggerBase::TriggerEvent_Implementation, );

   /** Can dynamically change what conditions is required for this trigger to fire off */
   UFUNCTION(BlueprintCallable)
   void SetConditions(TArray<URTSConditional*> NewConditions);

   UFUNCTION(BlueprintCallable)
   bool CheckActivationConditionsSatisfied();

   UFUNCTION(BlueprintCallable, BlueprintPure)
   bool GetEnabled() const { return bEnabled; }

   UFUNCTION(BlueprintCallable)
   void SetEnabled(bool bNewEnabled);

   UFUNCTION(BlueprintCallable, BlueprintPure)
   int GetTriggerLimit() const { return triggerLimit; }

   UFUNCTION(BlueprintCallable)
   void SetTriggerLimit(int NewTriggerLimit);

   UFUNCTION(BlueprintCallable)
   void SetTriggerName(FName NewName) { triggerName = NewName; }

   UFUNCTION(BlueprintCallable, BlueprintPure)
   FName GetTriggerName() const { return triggerName; }

 protected:
   class AUserInput*   GetControllerRef() const;
   class ARTSGameMode* GetGameModeRef() const;

   UPROPERTY(EditAnywhere)
   FName triggerName;

   /** Can dynamically enable/disable triggers to prevent them from firing */
   UPROPERTY(EditAnywhere)
   bool bEnabled = true;

   /** How many times this trigger can be activated, -1 for infinite number of activations */
   UPROPERTY(EditAnywhere)
   int triggerLimit = -1;

   /** List of conditions the trigger must comply with to activate */
   UPROPERTY(EditAnywhere, Instanced)
   TArray<URTSConditional*> activationConditions;
};

UCLASS(Abstract, BlueprintType, Blueprintable)
class MYPROJECT_API URTSTrigger : public URTSTriggerBase
{
   GENERATED_BODY()

 public:
   virtual void TriggerEvent_Implementation() override;
};

/** Composite patter holds a sequence of URTSTriger(s) to activate. */
UCLASS(Abstract, BlueprintType, Blueprintable)
class MYPROJECT_API URTSTriggerSequence : public URTSTriggerBase
{
   GENERATED_BODY()

 public:
   URTSTriggerSequence();

   void                     AddTrigger(URTSTriggerBase* TriggerToAdd);
   void                     RemoveTrigger(URTSTriggerBase* TriggerToRemove);
   TArray<URTSTriggerBase*> GetTriggersInSequence() const { return triggersInSequence; }
   virtual void             TriggerEvent_Implementation() override;

 protected:
   UPROPERTY(EditAnywhere, Instanced)
   TArray<URTSTriggerBase*> triggersInSequence;
};
