// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "EnvQueryTest_IsEnemy.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Unit.h"
#include "WorldObjects/Enemies/Enemy.h"

DECLARE_FLOAT_COUNTER_STAT(TEXT("EnvQuery"), Stat_EnvQuery_IsEnemy, STATGROUP_AI_EQS);

UEnvQueryTest_IsEnemy::UEnvQueryTest_IsEnemy()
{
	Cost = EEnvTestCost::Low;
	TestPurpose = EEnvTestPurpose::Filter;
	FilterType = EEnvTestFilterType::Match;
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
}

void UEnvQueryTest_IsEnemy::RunTest(FEnvQueryInstance& QueryInstance) const
{
	SCOPE_CYCLE_COUNTER(Stat_EnvQuery_IsEnemy);

	//make sure we have an owner for this test instance
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		float score = 0;
		const AActor* itemActor = GetItemActor(QueryInstance, It.GetIndex());
		if(const AUnit* unitRef = Cast<AUnit>(itemActor))
		{
			if(QueryOwner->GetClass()->IsChildOf(AEnemy::StaticClass()))
			{
				It.SetScore(TestPurpose, FilterType, unitRef->GetIsEnemy(), false);
			}
			else
			{
				It.SetScore(TestPurpose, FilterType, unitRef->GetIsEnemy(), true);
			}
			
		}
	}
}

FText UEnvQueryTest_IsEnemy::GetDescriptionTitle() const
{
	return NSLOCTEXT("EQTest", "IsEnemyDesc", "Is Unit An Enemy of Mine?");
}

FText UEnvQueryTest_IsEnemy::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}



