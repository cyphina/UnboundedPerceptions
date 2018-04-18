// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Tasks/BTTask_Support.h"

#include "ResourceManager.h"

#include "WorldObjects/Unit.h"
#include "UnitController.h"

#include "BehaviorTree/BTFunctionLibrary.h"
#include "EnvironmentQuery/EnvQueryManager.h"

#include "SpellSystem/MySpell.h"
#include "GameplayAbilityTargetTypes.h"


UBTTask_Support::UBTTask_Support()
{
	NodeName = "Support";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Support::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	FBTSupportTaskMemory* myMemory = (FBTSupportTaskMemory*)nodeMemory;

	myMemory->unitControllerRef = Cast<AUnitController>(ownerComp.GetAIOwner());
	myMemory->unitRef = myMemory->unitControllerRef->GetUnitOwner();

	//auto pred = [&](TSubclassOf<UMySpell> supportSpell)
	//{
	//	if(IsValid(supportSpell))
	//		return supportSpell.GetDefaultObject()->AbilityTags.HasAny(ResourceManager::supportTags) && !supportSpell.GetDefaultObject()->isOnCD(myMemory->unitRef->GetAbilitySystemComponent());		
	//	return false;
	//};

	//If we have any support spells equipped and they aren't on cd, grab first one
	//TArray<TSubclassOf<UMySpell>> supportSpells = myMemory->unitRef->abilities.FilterByPredicate(pred);

	//Get support spells off cooldown
	for (int i = 0; i < myMemory->unitRef->abilities.Num(); ++i)
	{
		if (IsValid(myMemory->unitRef->abilities[i]))
		{
			if (myMemory->unitRef->abilities[i].GetDefaultObject()->AbilityTags.HasAny(ResourceManager::supportTags) && !myMemory->unitRef->abilities[i].GetDefaultObject()->isOnCD(myMemory->unitRef->GetAbilitySystemComponent()))
			{
				myMemory->supportSpell = myMemory->unitRef->abilities[i].GetDefaultObject();
				myMemory->supportSpellIndex = i;
				break;
			}
		}
	}

	if (IsValid(myMemory->supportSpell))
	{
		//if its an aoe spell find the best part by running the AOE EQS
		if (myMemory->supportSpell->GetTargetting() == FGameplayTag::RequestGameplayTag("Skill.Targetting.Area"))
		{
			myMemory->unitControllerRef->FindBestAOELocation(myMemory->supportSpellIndex);
		}
		else if (myMemory->supportSpell->GetTargetting() == FGameplayTag::RequestGameplayTag("Skill.Targetting.None"))
		{
			myMemory->unitRef->BeginCastSpell(myMemory->supportSpellIndex, FGameplayAbilityTargetDataHandle());
		}
		else
		{
			myMemory->unitControllerRef->FindWeakestTarget(myMemory->supportSpellIndex);
		}
		return EBTNodeResult::InProgress;

	}
	return EBTNodeResult::Failed;
}

void UBTTask_Support::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* nodeMemory, float DeltaSeconds)
{
	//if spell finally casted...
	FBTSupportTaskMemory* myMemory = (FBTSupportTaskMemory*)nodeMemory;
	if(myMemory->supportSpell->isOnCD(myMemory->unitControllerRef->GetUnitOwner()->GetAbilitySystemComponent()))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 UBTTask_Support::GetInstanceMemorySize() const
{
	return sizeof(FBTSupportTaskMemory);
}




