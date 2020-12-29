// Created 11/29/20 3:13 AM

#include "MyProject.h"
#include "RiskComponent.h"

#include "Unit.h"
#include "UserInput.h"
#include "RTSGameState.h"
#include "TargetComponent.h"
#include "UpResourceManager.h"

void URiskComponent::BeginPlay()
{
   controllerRef = Cast<AUserInput>(UGameplayStatics::GetGameInstance(GetWorld())->GetFirstLocalPlayerController());
}

float URiskComponent::CalculateTargetRisk()
{
   int targetNum = 0;
   for(AUnit* e : controllerRef->GetGameState()->GetVisibleEnemies()) {
      if(e->targetComponent->GetTargetUnit() == GetOwner()) ++targetNum;
   }

   const float targetRiskValue = FMath::Clamp(UpResourceManager::DiminishFunc(targetNum, 2), 0.f, 1.f);
   return targetRiskValue;
}
