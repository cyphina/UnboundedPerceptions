#include "MyProject.h"
#include "UpAIHelperLibrary.h"

#include "Unit.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "MySpell.h"
#include "SpellCastComponent.h"

bool UUpAIHelperLibrary::IsTargetInRange(const AActor* referenceActor, const FVector& targetLocation, const float range)
{
   const FVector currentLocation = referenceActor->GetActorLocation();
   FVector       difference      = currentLocation - targetLocation;
   difference.Z                  = 0;

   if(FVector::DotProduct(difference, difference) <= range * range + SMALL_NUMBER) return true;
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
   if(otherUnits.Num() > 0) {
      float closestDistance = TNumericLimits<int>::Max();

      for(AUnit* otherUnit : otherUnits) {
         const float distToOtherUnit = FVector::Dist2D(referenceLocation, otherUnit->GetActorLocation());
         if(distToOtherUnit < closestDistance) {
            closestDistance = distToOtherUnit;
            closestUnit     = otherUnit;
         }
      }
   }
   return closestUnit;
}

void UUpAIHelperLibrary::AIBeginCastSpell(UEnvQuery* targetFindingQuery, TSubclassOf<UMySpell> spellToCast, USpellCastComponent* spellCastComponent)
{
   FEnvQueryRequest                                                     queryRequest{targetFindingQuery, spellCastComponent->GetOwner()};
   const auto                                                           castSpellAfterQuery = FQueryFinishedSignature::CreateLambda(
   [&spellToCast, spellCastComponent](const TSharedPtr<FEnvQueryResult> res) {
      spellToCast.GetDefaultObject()->GetTargeting().GetDefaultObject()->HandleQueryResult(res,
                                                                        Cast<AUnit>(spellCastComponent->GetOwner()), spellCastComponent, spellToCast);
   });
   queryRequest.Execute(EEnvQueryRunMode::SingleResult, castSpellAfterQuery);
}
