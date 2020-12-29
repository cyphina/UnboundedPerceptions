// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "AllyAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "UserInput.h"
#include "RTSPawn.h"

#include "UI/HUDManager.h"
#include "UI/UserWidgets/RTSIngameWidget.h"

#include "WorldObjects/Ally.h"
#include "WorldObjects/Unit.h"

#include "InteractableBase.h"

#include "SpellSystem/MySpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ManualSpellComponent.h"
#include "RTSStateComponent.h"
#include "SpellDataLibrary.h"
#include "SpellTargetingTypes.h"
#include "TargetComponent.h"
#include "UpStatComponent.h"

AAllyAIController::AAllyAIController()
{
   SetActorTickInterval(0.2f);
   behaviorTrees.SetNum(NUM_BEHAVIORAL_MODES);

   patrolComp = CreateDefaultSubobject<UPatrolComponent>("PatrolComponent");
   stateComp  = CreateDefaultSubobject<URTSStateComponent>("StateComponent");
}

void AAllyAIController::OnPossess(APawn* InPawn)
{
   Super::OnPossess(InPawn);
   allyRef             = Cast<AAlly>(GetPawn());
   currentAllyBehavior = AllyBehavioralMode::ABM_Neutral;
}

void AAllyAIController::OnUnPossess()
{
   spellCastComponent->OnSpellCasted().RemoveAll(this);
}

void AAllyAIController::SwitchAIModes(AllyBehavioralMode newMode)
{
   if(currentAllyBehavior != AllyBehavioralMode::ABM_Neutral) behaviorTreeComp->StopTree();
   // if we choose neutral there's no behavior tree since it's element in the tree array is empty
   if(behaviorTrees[static_cast<uint8>(newMode)]) {
      UseBlackboard(behaviorTrees[static_cast<uint8>(newMode)]->BlackboardAsset, blackboardComp);
      behaviorTreeComp->RestartTree(); // restart tree or we could be stuck on tasks that can't complete if they are stopped in progress
      behaviorTreeComp->StartTree(*behaviorTrees[static_cast<uint8>(newMode)]);
   }
   currentAllyBehavior = newMode;
}

void AAllyAIController::BeginAttack(AUnit* target)
{
   if(IsValid(target) && USpellDataLibrary::IsAttackable(*target->GetAbilitySystemComponent())) {
      if(USpellDataLibrary::IsStunned(*allyRef->GetAbilitySystemComponent())) {
         Stop();
         GetStateComponent()->ChangeState(EUnitState::STATE_ATTACKING);
         allyRef->targetComponent->SetTarget(target);
         AdjustPosition(allyRef->statComponent->GetMechanicAdjValue(EMechanics::AttackRange), target, [this]() { PrepareAttack(); });
      }
   } else {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), INVALID_TARGET_TEXT);
   }
}
