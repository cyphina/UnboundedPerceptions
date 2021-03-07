#include "MyProject.h"
#include "UpAIHelperLibrary.h"

#include "Unit.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "MySpell.h"
#include "SpellCastComponent.h"
#include "UnitController.h"

namespace AIHelperCVars
{
   static const int32 SOME_RANDOM_ID = 13456141;

   bool                           bPrintRange = false;
   static FAutoConsoleVariableRef CVarPrintRange(TEXT("RTSDebug.PrintRange"), bPrintRange, TEXT("When checking to see if target is in range, logs the range."));
}

bool UUpAIHelperLibrary::IsTargetInRange(const AActor* referenceActor, const FVector& targetLocation, const float range)
{
   const FVector currentLocation = referenceActor->GetActorLocation();
   FVector       difference      = currentLocation - targetLocation;
   difference.Z                  = 0;

   const float squaredDist = FVector::DotProduct(difference, difference);

#if UE_EDITOR
   if(AIHelperCVars::bPrintRange)
   {
      GEngine->AddOnScreenDebugMessage(AIHelperCVars::SOME_RANDOM_ID, 2.f, FColor::White,
                                       "Dist: " + FString::SanitizeFloat(squaredDist) + " Range: " + FString::SanitizeFloat(range * range));
   }
#endif

   if(squaredDist <= range * range + SMALL_NUMBER) return true;
   return false;
}

bool UUpAIHelperLibrary::IsTargetInRangeOfActor(const AActor* referenceActor, const AActor* targetActor, const float range)
{
   const FVector currentLocation = referenceActor->GetActorLocation();
   const FVector endLocation     = targetActor->GetActorLocation();
   FVector       difference      = currentLocation - endLocation;
   difference.Z                  = 0;

   const float squaredDist   = FVector::DotProduct(difference, difference);
   const float adjustedRange = range + referenceActor->GetSimpleCollisionRadius() + targetActor->GetSimpleCollisionRadius();

#if UE_EDITOR
   if(AIHelperCVars::bPrintRange)
   {
      GEngine->AddOnScreenDebugMessage(AIHelperCVars::SOME_RANDOM_ID, 2.f, FColor::White,
                                       "Dist: " + FString::SanitizeFloat(squaredDist) + " Adjusted Range: " + FString::SanitizeFloat(adjustedRange * adjustedRange));
   }
#endif

   if(squaredDist <= adjustedRange * adjustedRange + SMALL_NUMBER) return true;
   return false;
}

bool UUpAIHelperLibrary::IsFacingTarget(const AActor* referenceActor, const FVector& targetLocation, float angleErrorCutoff)
{
   const FVector difference = (targetLocation - referenceActor->GetActorLocation()).GetSafeNormal2D();
   const float   dot        = FVector::DotProduct(referenceActor->GetActorForwardVector(), difference);

   // .05 is 18 degrees lenient by default (only from right side).
   if(dot > 1.f - angleErrorCutoff) return true;
   return false;
}

FQuat UUpAIHelperLibrary::FindLookRotation(const AActor* referenceActor, const FVector& targetPoint)
{
   const FVector difference         = targetPoint - referenceActor->GetActorLocation();
   const FVector projectedDirection = FVector(difference.X, difference.Y, 0);
   return FRotationMatrix::MakeFromX(FVector(projectedDirection)).ToQuat();
}

AUnit* UUpAIHelperLibrary::FindClosestUnit(const FVector referenceLocation, const TSet<AUnit*>& otherUnits)
{
   AUnit* closestUnit = nullptr;
   if(otherUnits.Num() > 0)
   {
      float closestDistance = TNumericLimits<int>::Max();

      for(AUnit* otherUnit : otherUnits)
      {
         const float distToOtherUnit = FVector::Dist2D(referenceLocation, otherUnit->GetActorLocation());
         if(distToOtherUnit < closestDistance)
         {
            closestDistance = distToOtherUnit;
            closestUnit     = otherUnit;
         }
      }
   }
   return closestUnit;
}

void UUpAIHelperLibrary::AIBeginCastSpell(UEnvQuery* targetFindingQuery, const TSubclassOf<UMySpell> spellToCast, USpellCastComponent* spellCastComponent)
{
   if(AUnitController* unitController = Cast<AUnitController>(spellCastComponent->GetOwner()))
   {
      FEnvQueryRequest queryRequest{targetFindingQuery, unitController->GetUnitOwner()};

      if(UMySpell* spellCDO = spellToCast.GetDefaultObject())
      {
         const float aoe = spellCDO->GetAOE(unitController->GetUnitOwner()->GetAbilitySystemComponent());
         if(aoe > 0)
         {
            queryRequest.SetFloatParam("radius", aoe);
         }

         const auto castSpellAfterQuery =
             FQueryFinishedSignature::CreateLambda([spellToCast, spellCastComponent, spellCDO, unitController](const TSharedPtr<FEnvQueryResult> res) {
                if(spellToCast)
                {
                   if(const TSubclassOf<UUpSpellTargeting> targetingStrategyClass = spellCDO->GetTargeting())
                   {
                      if(UUpSpellTargeting* targetingStrategy = targetingStrategyClass.GetDefaultObject())
                      {
                         targetingStrategy->HandleQueryResult(res, unitController->GetUnitOwner(), spellCastComponent, spellToCast);
                      }
                   }
                }
             });
         queryRequest.Execute(EEnvQueryRunMode::SingleResult, castSpellAfterQuery);
      }
   }
}
