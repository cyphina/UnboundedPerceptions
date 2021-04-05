// Created 2/28/21 07:42 AM

#include "MyProject.h"
#include "RTSForceStaffCalc.h"
#include "Unit.h"
#include "UnitController.h"

URTSForceStaffCalc::URTSForceStaffCalc(const FObjectInitializer& objectInitializer) :
   Super(objectInitializer)
{
   RelevantAttributesToCapture.Add(URTSAbilitySystemComponent::GetEffectParameterCapture());
}

void URTSForceStaffCalc::Execute_Implementation
(const FGameplayEffectCustomExecutionParameters& executionParams, FGameplayEffectCustomExecutionOutput& outExecutionOutput) const
{
   UAbilitySystemComponent* ownerComponent  = executionParams.GetSourceAbilitySystemComponent();
   UAbilitySystemComponent* targetComponent = executionParams.GetTargetAbilitySystemComponent();

   AUnit* sourceUnit = nullptr;
   AUnit* targetUnit = nullptr;

   if(ownerComponent)
   {
      sourceUnit = Cast<AUnit>(ownerComponent->AvatarActor);
   }
   if(targetComponent)
   {
      targetUnit = Cast<AUnit>(targetComponent->AvatarActor); 
   }

   if(sourceUnit && targetUnit)
   {
      if(sourceUnit != targetUnit)
      {
         if(AUnitController* targetUnitController = targetUnit->GetUnitController())
         {
            targetUnitController->PauseMove(targetUnitController->GetCurrentMoveRequestID());

            float effectPower = 0; 
            for(FGameplayEffectAttributeCaptureDefinition captureDef : RelevantAttributesToCapture)
            {
               executionParams.AttemptCalculateCapturedAttributeMagnitude(captureDef, FAggregatorEvaluateParameters(), effectPower);
            }

            const FVector launchVector = (targetUnit->GetActorLocation() - sourceUnit->GetActorLocation()).GetUnsafeNormal() * effectPower;
            targetUnit->LaunchCharacter(launchVector, true, true);

            FTimerHandle pushBackResumeMoveTimer;
            sourceUnit->GetWorld()->GetTimerManager().SetTimer(pushBackResumeMoveTimer, [targetUnitController]()
            {
               targetUnitController->ResumeMove(targetUnitController->GetCurrentMoveRequestID());   
            }, 0.15f, false, 0.f);         
         }
      }
   }
}
