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

AAllyAIController::AAllyAIController()
{
   SetActorTickInterval(0.2f);
   behaviorTrees.SetNum(NUM_BEHAVIORAL_MODES);
}

void AAllyAIController::OnPossess(APawn* InPawn)
{
   Super::OnPossess(InPawn);
   allyRef             = Cast<AAlly>(GetPawn());
   currentAllyBehavior = AllyBehavioralMode::ABM_Neutral;
}

void AAllyAIController::OnPerceptionUpdated(TArray<AActor*> updatedActors)
{
}

void AAllyAIController::SwitchAIModes(AllyBehavioralMode newMode)
{
   if(currentAllyBehavior != AllyBehavioralMode::ABM_Neutral)
      behaviorTreeComp->StopTree();
   // if we choose neutral there's no behavior tree since it's element in the tree array is empty
   if(behaviorTrees[static_cast<uint8>(newMode)]) {
      UseBlackboard(behaviorTrees[static_cast<uint8>(newMode)]->BlackboardAsset, blackboardComp);
      behaviorTreeComp->RestartTree(); // restart tree or we could be stuck on tasks that can't complete if they are stopped in progress
      behaviorTreeComp->StartTree(*behaviorTrees[static_cast<uint8>(newMode)]);
   }
   currentAllyBehavior = newMode;
}

void AAllyAIController::Stop()
{
   Super::Stop();
   currentlySelectedSpell = nullptr;
}

void AAllyAIController::BeginAttack(AUnit* target)
{
   if(IsValid(target) && target->GetCanTarget()) {
      if(!allyRef->IsStunned()) {
         Stop(); // Stop any other actions we're doing
         allyRef->state->ChangeState(EUnitState::STATE_ATTACKING);
         allyRef->SetTargetUnit(target);
         // GetUnitOwner()->targetData.spellTargetData    = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(target);
         allyRef->unitProperties.finishedTurningAction.BindUObject(this, &AAllyAIController::PrepareAttack);
         if(AdjustPosition(allyRef->GetMechanicAdjValue(EMechanics::AttackRange), target))
            allyRef->unitProperties.finishedTurningAction.Execute();
      }
   } else {
      GetCPCRef()->GetHUDManager()->GetIngameHUD()->DisplayHelpText(AAlly::invalidTargetText);
   }
}

bool AAllyAIController::BeginCastSpell(TSubclassOf<UMySpell> spellToCast)
{
   // If we ever directly call this in our ally behavioral tree, make sure the spell class set to this task is actually one we have equipped
   const int spellToCastAbilityIndex = allyRef->abilities.Find(spellToCast);
   if(spellToCastAbilityIndex != INDEX_NONE) {
      allyRef->SetSpellIndex(spellToCastAbilityIndex); // Set this here because player could press another button while AI spell is being casted leading to wrong spell CD
      if(Super::BeginCastSpell(spellToCast)) {
         return true;
      }
   }
   return false;
}

void AAllyAIController::DeselectSpell()
{
   currentlySelectedSpell = nullptr;
   GetCPCRef()->GetCameraPawn()->HideSpellCircle();
   GetCPCRef()->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Select);
}

bool AAllyAIController::PressedCastSpell(TSubclassOf<UMySpell> spellToCast)
{
   UMySpell* spell = spellToCast.GetDefaultObject();
   if(IsValid(spell)) // Check for valid spell class
   {
      if(!spell->isOnCD(allyRef->GetAbilitySystemComponent())) {
         if(spell->GetCost(allyRef->GetAbilitySystemComponent()) <= allyRef->GetVitalCurValue(EVitals::Mana)) {
            if(currentlySelectedSpell == spellToCast) // If already selected
            {
               DeselectSpell();
               return false;
            }

            if(spell->GetTargetting().GetTagName() == "Skill.Targetting.None") // Non-targetted?  Then just cast it
            {
               Stop();
               SetupDelayedSpellProps(spellToCast);
               IncantationCheck(spellToCast);
            } else {
               GetCPCRef()->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Magic); // Set wand targetting cursor
            }

            // Set our current spell to spellToCast for recording purposes
            currentlySelectedSpell = spellToCast;

            if(spell->GetTargetting().MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Area"))) {
               // TODO: depending on the spell area targetting, use different indicators
               // If it's an AOE spell show the targetting indicator
               GetCPCRef()->GetCameraPawn()->ShowSpellCircle(spell->GetAOE(allyRef->GetAbilitySystemComponent()));
            } else {
               GetCPCRef()->GetCameraPawn()->HideSpellCircle();
            }

            return true;
         } else // If not enough mana
         {
            GetCPCRef()->GetHUDManager()->GetIngameHUD()->DisplayHelpText(allyRef->notEnoughManaText);
            return false;
         }
      } else // If spell still not ready
      {
         GetCPCRef()->GetHUDManager()->GetIngameHUD()->DisplayHelpText(allyRef->onCooldownText);
         return false;
      }
   }
   return false;
}

bool AAllyAIController::PressedCastSpell(int spellCastingIndex)
{
   currentlySelectedSpellIndex = spellCastingIndex;
   bool spellSelected          = PressedCastSpell(allyRef->abilities[spellCastingIndex]);
   return spellSelected;
}

bool AAllyAIController::InvalidTarget() const
{
   allyRef->controllerRef->GetHUDManager()->GetIngameHUD()->DisplayHelpText(allyRef->invalidTargetText);
   return false;
}

bool AAllyAIController::CheckUnitTarget(FName spellTargetting, AUnit* unit) const
{
   if(UNLIKELY(unit->GetIsEnemy())) { // Mostly used by allies except in testing modes
      // Check to see if target is meets the criteria of the spell
      if(spellTargetting == "Skill.Targetting.Single.Friendly") {
         return InvalidTarget();
      }
   } else {
      if(spellTargetting == "Skill.Targetting.Single.Enemy") {
         return InvalidTarget();
      }
   }
   return true;
}

void AAllyAIController::CastSpellOnSelf(TSubclassOf<UMySpell> spellClass)
{
   Stop();
   SetupDelayedSpellProps(spellClass);
   IncantationCheck(spellClass);
   GetCPCRef()->GetCameraPawn()->SetSecondaryCursor();
}

void AAllyAIController::AreaManualTargetting(FHitResult& result, TSubclassOf<UMySpell> spellClass, UMySpell* spell)
{
   // If we clicked on a unit, target the unit for the cast
   if(AUnit* unitRef = Cast<AUnit>(result.Actor.Get())) {
      // If we're in range already or we're casting it on ourself
      if(IsTargettingSelf(unitRef))
         CastSpellOnSelf(spellClass);

      // Else target the unit and move to position
      FinalizeSpellTargetting(spellClass, unitRef);
      GetCPCRef()->GetCameraPawn()->HideSpellCircle();
   }
   // Else cast on the ground
   else {
      FinalizeSpellTargetting(spellClass, result.Location);
      GetCPCRef()->GetCameraPawn()->HideSpellCircle();
   }
}

bool AAllyAIController::UnitManualTargetting(FHitResult& result, TSubclassOf<UMySpell> spellClass, FName spellTargetting)
{
   if(spellTargetting != "Skill.Targetting.Single.Interactable") {
      AUnit* unitRef = Cast<AUnit>(result.Actor.Get());
      if(unitRef->GetCanTarget()) {
         if(!CheckUnitTarget(spellTargetting, unitRef))
            return false;

         // If casting on ourselves, then we can just instantly cast
         if(IsTargettingSelf(unitRef)) {
            CastSpellOnSelf(spellClass);
            return true;
         }

         FinalizeSpellTargetting(spellClass, unitRef);

      } else // Invulnerable unit
      {
         return InvalidTarget();
      }
   } else // We tried to target a unit with an interactable spell
   {
      return InvalidTarget();
   }
   return false;
}

bool AAllyAIController::InteractableManualTargetting(FHitResult& result, TSubclassOf<UMySpell> spellClass, FName spellTargetting)
{
   if(spellTargetting != "Skill.Targetting.Single.Interactable" && spellTargetting != "Skill.Targetting.Single.AllUnitsAndInteractables") {
      return InvalidTarget();
   }

   FinalizeSpellTargetting(spellClass, result.Actor.Get());
   return true;
}

bool AAllyAIController::SetupSpellTargetting(UPARAM(ref) FHitResult& result, TSubclassOf<UMySpell> spellClass)
{
   UMySpell*   spell           = spellClass.GetDefaultObject();
   const FName spellTargetting = spell->GetTargetting().GetTagName();

   if(IsValid(spell) && result.IsValidBlockingHit()) {
      // Handle any spells that can target the ground first
      if(spellTargetting == "Skill.Targetting.Area") {
         AreaManualTargetting(result, spellClass, spell);
      } else {
         // Targetting a single object
         if(IsValid(result.Actor.Get())) {
            // If we clicked on a unit, we must be using a single target spell.
            // Make sure actor is a unit and that it is visible (we have vision over the target).
            // This may already be handled for us since we turn off collision when toggling a unit's visibility
            if(result.Actor->GetClass()->IsChildOf(AUnit::StaticClass())) {
               if(!UnitManualTargetting(result, spellClass, spellTargetting))
                  return false;
            }
            // If we are targetting an interactable
            else if(result.Actor->GetClass()->IsChildOf(AInteractableBase::StaticClass())) {
               if(!InteractableManualTargetting(result, spellClass, spellTargetting))
                  return false;
            }
            // If we clicked on something that isn't an interactable or unit, and our targetting isn't a location
            else {
               return InvalidTarget();
            }
         } else {
            // If our click trace failed
            return InvalidTarget();
         }
      }
      return true;
   }
   return false;
}

void AAllyAIController::SetupDelayedSpellProps(TSubclassOf<UMySpell> spellToCast) const
{
   allyRef->SetCurrentSpell(spellToCast);
   allyRef->SetSpellIndex(currentlySelectedSpellIndex);
}

void AAllyAIController::FinalizeSpellTargetting(TSubclassOf<UMySpell> spellClass, AActor* targetActor)
{
   Stop();
   allyRef->SetTargetActor(targetActor);
   GetCPCRef()->GetCameraPawn()->SetSecondaryCursor(); // Just set cursor to to select so the cursor check loop will quickly change the cursor back to normal
   SetupDelayedSpellProps(spellClass);                 // Reassign these variables that were cleared since we had to stop our current action

   AdjustCastingPosition(spellClass, targetActor);
}

void AAllyAIController::FinalizeSpellTargetting(TSubclassOf<UMySpell> spellClass, AUnit* targetUnit)
{
   Stop(); // Stop whatever we were doing currently
   allyRef->SetTargetUnit(targetUnit);
   GetCPCRef()->GetCameraPawn()->SetSecondaryCursor(); // Just set cursor to to select so the cursor check loop will quickly change the cursor back to normal
   SetupDelayedSpellProps(spellClass);                 // Reassign these variables that were cleared since we had to stop our current action
   AdjustCastingPosition(spellClass, targetUnit);
}

void AAllyAIController::FinalizeSpellTargetting(TSubclassOf<UMySpell> spellClass, FVector targetLocation)
{
   Stop();
   allyRef->SetTargetLocation(targetLocation);
   GetCPCRef()->GetCameraPawn()->SetSecondaryCursor(); // Just set cursor to to select so the cursor check loop will quickly change the cursor back to normal
   SetupDelayedSpellProps(spellClass);                 // Reassign these variables that were cleared since we had to stop our current action
   AdjustCastingPosition(spellClass, targetLocation);
}
