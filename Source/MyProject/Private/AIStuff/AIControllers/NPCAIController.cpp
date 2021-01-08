// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "NPCAIController.h"
#include "AIModule/Classes/BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "WorldObjects/BaseHero.h"

const FName ANPCAIController::targetKeyName = "target";

ANPCAIController::ANPCAIController() {}

void ANPCAIController::OnPossess(APawn* inPawn) { Super::OnPossess(inPawn); }

void ANPCAIController::BeginPlay()
{
   Super::BeginPlay();
   UseBlackboard(npcBB, blackboardComp);
}

void ANPCAIController::Follow(const ABaseHero* heroToFollow)
{
   blackboardComp->SetValueAsObject(targetKeyName, const_cast<ABaseHero*>(heroToFollow));
   RunBehaviorTree(followTree);
}

void ANPCAIController::Patrol() { RunBehaviorTree(patrolTree); }

void ANPCAIController::Stop() { GetBrainComponent()->StopLogic(FString("NPC Stop")); }
