// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "FindEnemyQueryContext.h"
#include "AIStuff/AIControllers/UnitController.h"

//class used to provide us with a target to trace to 
void UFindEnemyQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	//Get owner of this Query and cast it to an actor
	//Then get the actor's controller and cast it to AIController
	AUnitController* AICon = Cast<AUnitController>(Cast<AActor>(QueryInstance.Owner.Get())->GetInstigatorController());
	if(AICon && AICon->GetUnitOwner()->GetTargetData().IsValid(0))
	{
		//Set the context SeeingPawn to provided context data
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, AICon->GetUnitOwner()->GetTargetData().Get(0)->GetActors()[0].Get());
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Wee") + AICon->GetSeeingPawn()->GetActorLocation().ToString());
	}
}


