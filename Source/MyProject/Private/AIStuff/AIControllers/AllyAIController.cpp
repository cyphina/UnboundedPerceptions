// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "AllyAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "UserInput.h"
#include "RTSPawn.h"

#include "UI/HUDManager.h"

#include "WorldObjects/Ally.h"
#include "WorldObjects/Unit.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "RTSStateComponent.h"

#include "TargetComponent.h"
#include "PatrolComponent.h"

#include "ManualSpellComponent.h"
#include "TargetedAttackComponent.h"
#include "SpellCastComponent.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"

const FText AAllyAIController::INVALID_TARGET_TEXT = NSLOCTEXT("Attacking", "InvalidTarget", "Cannot attack this target");

AAllyAIController::AAllyAIController(const FObjectInitializer& ObjectInitializer) : AUnitController(ObjectInitializer)
{
   SetActorTickInterval(0.2f);
   behaviorTrees.SetNum(NUM_BEHAVIORAL_MODES);

   patrolComp               = CreateDefaultSubobject<UPatrolComponent>("PatrolComponent");
   stateComp                = CreateDefaultSubobject<URTSStateComponent>("StateComponent");
   spellCastComponent       = CreateDefaultSubobject<USpellCastComponent>("SpellComponent");
   manualSpellCastComponent = CreateDefaultSubobject<UManualSpellComponent>("ManualSpellComponent");
   targetedAttackComponent  = CreateDefaultSubobject<UTargetedAttackComponent>("TargetedAttackComponent");
   blackboardComp           = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
   behaviorTreeComp         = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
}

void AAllyAIController::OnPossess(APawn* InPawn)
{
   Super::OnPossess(InPawn);
   allyRef = Cast<AAlly>(GetPawn());
}

void AAllyAIController::OnUnPossess()
{
   Super::OnUnPossess();
   spellCastComponent->OnSpellCasted().RemoveAll(this);
}

void AAllyAIController::SwitchAIModes(EAllyBehavioralMode newMode)
{
   behaviorTreeComp->StopTree();
   // If we choose neutral there's no behavior tree since it's element in the tree array is empty
   if(behaviorTrees[static_cast<uint8>(newMode)])
   {
      UseBlackboard(behaviorTrees[static_cast<uint8>(newMode)]->BlackboardAsset, blackboardComp);
      behaviorTreeComp->RestartTree(); // restart tree or we could be stuck on tasks that can't complete if they are stopped in progress
      behaviorTreeComp->StartTree(*behaviorTrees[static_cast<uint8>(newMode)]);
   }
}
