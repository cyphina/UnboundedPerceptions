#pragma once
#include "RTSConditional.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Up_Log_Conditionals, Log, All);

/** Base class for Composite pattern. Inherit from URTSConditional if making new conditionals */
UCLASS(Abstract, BlueprintType, Blueprintable)
class URTSConditionalBase : public UObject
{
   GENERATED_BODY()

 public:
   UFUNCTION(BlueprintNativeEvent)
   bool CheckIsConditionSatisfied();

   virtual bool CheckIsConditionSatisfied_Implementation() PURE_VIRTUAL(URTSConditional::CheckIsConditionSatisfied_Implementation, return true;);

   /** Inverse the result of this conditional */
   UPROPERTY(EditAnywhere)
   bool bReverseResult = false;

 protected:
   class AUserInput*   GetControllerRef() const;
   class ARTSGameMode* GetGameModeRef() const;
};

UCLASS(Abstract, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class URTSConditional : public URTSConditionalBase
{
   GENERATED_BODY()

 public:
   bool CheckIsConditionSatisfied_Implementation() override;
};

/** Composite patter holds a sequence of URTSConditional(s) to activate. */
UCLASS(Abstract, BlueprintType, Blueprintable)
class MYPROJECT_API URTSConditionalSequence : public URTSConditionalBase
{
   GENERATED_BODY()

 public:
   URTSConditionalSequence();

   void                     AddConditional(URTSConditional* ConditionalToAdd);
   void                     RemoveConditional(URTSConditional* ConditionalToRemove);
   TArray<URTSConditional*> GetConditionalsInSequence() const { return conditionalsInSequence; }
   bool                     CheckIsConditionSatisfied_Implementation() override;

 protected:
   UPROPERTY(EditAnywhere, Instanced)
   TArray<URTSConditional*> conditionalsInSequence;
};
