// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "UnitController.generated.h"

class AUnit;
class AUserInput;
class UEnvQuery;
class UMySpell;

/**
 * Base controller for all unit classes
 */

UCLASS()
class MYPROJECT_API AUnitController : public AAIController
{
   GENERATED_BODY()

   AUnit*        ownerRef = nullptr;
   AUserInput*   CPCRef   = nullptr;
   TSet<AUnit*>* groupRef; // part of what current group

 protected:
   /**blackboard key value name*/
   const FName blackboardEnemyKey = FName("Target");

 public:
   AUnitController();

   /** Location which our controller will patrol */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings")
   TArray<FVector> patrolLocations;

   /**Behaviortreecomponent used to start a behavior tree*/
   UBehaviorTreeComponent* behaviorTreeComp;

   /**blackboardcomponent used to initialize blackboard values and set/get values from a blackboard asset*/
   UBlackboardComponent* blackboardComp;

   /**Make sure to call UseBlackboard/InitializeBlackboard in the children classes depending on how they use their blackboards as well as starting the tree*/
   void Possess(APawn* InPawn) override;
   void BeginPlay() override;

   inline AUnit*      GetUnitOwner() const { return ownerRef; }
   inline AUserInput* GetCPCRef() const { return CPCRef; }

   /**
    *Find the best spot for targetting an AOE spell
    *@param isSupport - Find best AOE location to hit the most enemies (false) or friends (true)?
    */
   virtual void FindBestAOELocation(int spellIndex, bool isSupport);

   /**
    *Find the best unit to target a single spell
    *@param isSupport - Find best AOE location to hit the most enemies (false) or friends (true)?
    */
   virtual void FindWeakestTarget(int spellIndex, bool isSupport);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AIModes")
   FORCEINLINE int GetAggresionLevel() const { return aggressionLevel; }

 protected:
   /*Describes from 1-5 how aggressive the unit is (1 = Passive, 3 = Neutral, 5 = Aggresive).
    *Not all units (enemies) need the complexity of having 5 states so we have an int to represent this*/
   int aggressionLevel;

 private:
   /**Spell to cast after target is found*/
   int spellToCastIndex;

   /**Environment query to find the best place to cast an AOE spell for maximum target hits when healing*/
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* findBestAOESupportLocation;

   /**Environment query to find the best place to cast an AOE spell for maximum target hits when casting an AOE spell*/
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* findBestAOEAssaultLocation;

   /**Environment query to find the weakest allied unit.  Allies and enemies have should have different EQueries set*/
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* findWeakestAllyTarget;

   /**Environment query to find the weakest enemy unit.  Allies and enemies have should have different EQueries set*/
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* findWeakestEnemyTarget;

   void OnAOELocationFound(TSharedPtr<FEnvQueryResult> result);
   void OnWeakestTargetFound(TSharedPtr<FEnvQueryResult> result);
};
