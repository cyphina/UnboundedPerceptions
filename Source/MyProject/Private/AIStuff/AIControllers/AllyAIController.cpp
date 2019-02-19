// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "AllyAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "UserInput.h"
#include "RTSPawn.h"

#include "UI/HUDManager.h"
#include "UI/UserWidgets/MainWidget.h"

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

void AAllyAIController::Tick(float deltaTime)
{
   Super::Tick(deltaTime);
}

void AAllyAIController::Possess(APawn* InPawn)
{
   Super::Possess(InPawn);
   allyRef = Cast<AAlly>(GetPawn());
   currentAllyBehavior = AllyBehavioralMode::ABM_Neutral;
}

void AAllyAIController::OnPerceptionUpdated(TArray<AActor*> updatedActors)
{
}

void AAllyAIController::SwitchAIModes(AllyBehavioralMode newMode)
{
   if (currentAllyBehavior != AllyBehavioralMode::ABM_Neutral) behaviorTreeComp->StopTree();
   // if we choose neutral there's no behavior tree since it's element in the tree array is empty
   if (behaviorTrees[static_cast<uint8>(newMode)]) {
      UseBlackboard(behaviorTrees[static_cast<uint8>(newMode)]->BlackboardAsset, blackboardComp);
      behaviorTreeComp->RestartTree(); // restart tree or we could be stuck on tasks that can't complete if they are stopped in progress
      behaviorTreeComp->StartTree(*behaviorTrees[static_cast<uint8>(newMode)]);
   }
   currentAllyBehavior = newMode;
}

void AAllyAIController::Stop()
{
   Super::Stop();
}

void AAllyAIController::BeginAttack(AUnit* target)
{
   if (target->GetCanTarget()) {
      if (!allyRef->IsStunned()) {
         Stop(); // Stop any other actions we're doing
         allyRef->state->ChangeState(EUnitState::STATE_ATTACKING);
         allyRef->SetTarget(target);
         //GetUnitOwner()->targetData.spellTargetData    = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(target);
         allyRef->unitProperties.turnAction.BindUObject(this, &AAllyAIController::PrepareAttack);
         if (AdjustPosition(allyRef->GetMechanicAdjValue(static_cast<int>(Mechanics::AttackRange)), target))
            allyRef->unitProperties.turnAction.Execute();
      }
   }
   else {
      allyRef->controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(AAlly::invalidTargetText);
   }
}

bool AAllyAIController::BeginCastSpell(TSubclassOf<UMySpell> spellToCast, const FGameplayAbilityTargetDataHandle& targetData)
{
   //If we ever directly call this in our ally behavioral tree, make sure the spellclass set to this task is actually one we have equipped
   int spellToCastAbilityIndex = allyRef->abilities.Find(spellToCast);
   if (spellToCastAbilityIndex != INDEX_NONE)
   {
      allyRef->SetSpellIndex(spellToCastAbilityIndex);
      if (Super::BeginCastSpell(spellToCast, targetData)) { return true; }
   }
   return false;
}

bool AAllyAIController::PressedCastSpell(TSubclassOf<UMySpell> spellToCast)
{
   UMySpell* spell = spellToCast.GetDefaultObject();
   if (IsValid(spell) && allyRef->GetState() != EUnitState::STATE_CHANNELING) //Check for valid spell class and make sure not channeling anything currently
   {
      if (!spell->isOnCD(allyRef->GetAbilitySystemComponent())) {
         if (spell->GetCost(allyRef->GetAbilitySystemComponent()) <= allyRef->GetVitalCurValue(static_cast<int>(Vitals::Mana))) {
            if (allyRef->GetCurrentSpell() == spellToCast) //If already selected
            {
               allyRef->SetCurrentSpell(nullptr); //Deselect the spell
               GetCPCRef()->GetCameraPawn()->HideSpellCircle();
               GetCPCRef()->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Select);
               return false;
            }

            if (spell->GetTargetting().GetTagName() == "Skill.Targetting.None") //Non-targetted?  Then just cast it
            {
               Stop();
               allyRef->SetCurrentSpell(spellToCast);
               PreCastChannelingCheck(spellToCast);
            }
            else {
               GetCPCRef()->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Magic); //Set wand targetting cursor
            }

            //Set our current spell to spellToCast for recording purposes
            allyRef->SetCurrentSpell(spellToCast);

            if (spell->GetTargetting().MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Area"))) {
               // TODO: depending on the spell area targetting, use different indicators
               // if it's an AOE spell show the targetting indicator
               GetCPCRef()->GetCameraPawn()->ShowSpellCircle(spell->GetAOE(allyRef->GetAbilitySystemComponent()));
            }
            else {
               GetCPCRef()->GetCameraPawn()->HideSpellCircle();
            }

            return true;
         }
         else //If not enough mana
         {
            GetCPCRef()->GetHUDManager()->GetMainHUD()->DisplayHelpText(allyRef->notEnoughManaText);
            return false;
         }

      }
      else //If spell still not ready
      {
         GetCPCRef()->GetHUDManager()->GetMainHUD()->DisplayHelpText(allyRef->onCooldownText);
         return false;
      }
   }
   return false;
}

bool AAllyAIController::PressedCastSpell(int spellCastingIndex)
{
   bool spellSelected = PressedCastSpell(allyRef->abilities[spellCastingIndex]);
   if (spellSelected) allyRef->SetSpellIndex(spellCastingIndex);
   return spellSelected;
}

bool AAllyAIController::SetupSpellTargetting(FHitResult result, TSubclassOf<UMySpell> spellClass)
{
   UMySpell* spell = spellClass.GetDefaultObject();
   FName     spellTargetting = spell->GetTargetting().GetTagName();

   if (IsValid(spell)) {
      if (spellTargetting == "Skill.Targetting.Area") //We can target anything and the area around it will be affected
      {
         Stop();
         allyRef->targetData.spellTargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(result);
         //If we clicked on a unit, target the unit for the cast
         if (AUnit* unitRef = Cast<AUnit>(result.Actor.Get()))
         {
            allyRef->SetTarget(unitRef);

            FinalizeSpellTargetting(spellClass);
            if (AdjustPosition(spell->GetRange(allyRef->GetAbilitySystemComponent()), allyRef->targetData.targetActor))
               PreCastChannelingCheck(spellClass);
            return true;
         }
         else //Else cast on the ground
         {
            allyRef->targetData.targetLocation = result.Location;
            FinalizeSpellTargetting(spellClass);
            if (AdjustPosition(spell->GetRange(allyRef->GetAbilitySystemComponent()), allyRef->targetData.targetLocation))
               PreCastChannelingCheck(spellClass);
            return true;
         }
      }
      else //We need to check to see if we targetted the right kind of actor
      {
         if (IsValid(result.Actor.Get())) {
            //If we clicked on a unit, we must be using a single target spell.  Make sure actor is a unit and that it is visible (we have vision over the target)
            if (result.Actor->GetDefaultAttachComponent()->IsVisible() && result.Actor->GetClass()->IsChildOf(AUnit::StaticClass())) {
               if (spellTargetting != "Skill.Targetting.Single.Interactable") {
                  AUnit* unit = Cast<AUnit>(result.Actor.Get());
                  if (unit->GetCanTarget())
                  {
                     if (unit->GetIsEnemy()) { //Check to see if target is meets the criteria of the spell
                        if (spellTargetting == "Skill.Targetting.Single.Friendly") {
                           allyRef->controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(allyRef->invalidTargetText);
                           return false;
                        }
                     }
                     else
                     {
                        if (spellTargetting == "Skill.Targetting.Single.Enemy") {
                           allyRef->controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(allyRef->invalidTargetText);
                           return false;
                        }
                     }

                     //Keep track of targetting data
                     Stop();
                     allyRef->targetData.spellTargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(result);
                     allyRef->SetTarget(unit);

                     //If casting on ourselves, then we can just instantly cast
                     if (allyRef->targetData.targetUnit == allyRef) {
                        allyRef->SetCurrentSpell(spellClass); //Set it again for now since we had to stop
                        PreCastChannelingCheck(spellClass);
                        GetCPCRef()->GetCameraPawn()->ChangeCursor(ECursorStateEnum::Select);
                        return true;
                     }
                  }
               }
            }
            //If we are targetting an interactable
            else if (result.Actor->GetClass()->IsChildOf(AInteractableBase::StaticClass())) {
               if (spellTargetting != "Skill.Targetting.Single.Interactable" && spellTargetting != "Skill.Targetting.Single.AllUnitsAndInteractables") { //Make sure our spell works on that  

                  GetCPCRef()->GetHUDManager()->GetMainHUD()->DisplayHelpText(allyRef->invalidTargetText);
                  return false;
               }

               Stop();
               allyRef->targetData.targetActor = Cast<AInteractableBase>(result.Actor);
               //Set the targetlocation as the actor location since logically you're casting the spell on the object (not the interactable location)
               allyRef->targetData.targetLocation = allyRef->targetData.targetActor->GetActorLocation();
               allyRef->targetData.spellTargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(result);
            }
            else { //If we clicked on something that isn't an interactable or unit, and our targetting isn't a location
               GetCPCRef()->GetHUDManager()->GetMainHUD()->DisplayHelpText(allyRef->invalidTargetText);
               return false;
            }
         }
         else { //If our click trace failed
            GetCPCRef()->GetHUDManager()->GetMainHUD()->DisplayHelpText(allyRef->invalidTargetText);
            return false;
         }

         FinalizeSpellTargetting(spellClass);
         if (AdjustPosition(spell->GetRange(allyRef->GetAbilitySystemComponent()), allyRef->targetData.targetActor))
            PreCastChannelingCheck(spellClass);
         return true;
      }
   }
   return false;
}

void AAllyAIController::FinalizeSpellTargetting(TSubclassOf<UMySpell> spellClass)
{
   GetCPCRef()->GetCameraPawn()->ChangeCursor(ECursorStateEnum::Select); //Just set cursor to to select so the loop will quickly change the cursor back to normal
   auto castTurnAction = [this, spellClass]() { PreCastChannelingCheck(spellClass);};
   allyRef->unitProperties.turnAction.BindLambda(castTurnAction);
   allyRef->SetCurrentSpell(spellClass); //Set it again for now since we had to stop
   allyRef->state->ChangeState(EUnitState::STATE_CASTING);
}
