// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCAIController.generated.h"

class ABaseHero;

/**
 * Controller for more active NPCs.  Some NPCs just stand around and talk, but these guys are moving about, and maybe following you for some reasons
 */
UCLASS()
class MYPROJECT_API ANPCAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ANPCAIController();
	void Possess(APawn* inPawn) override;
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "NPCActions")
	void Follow(ABaseHero* heroToFollow);

	UFUNCTION(BlueprintCallable, Category = "NPCActions")
	void Patrol();

	UFUNCTION(BlueprintCallable, Category = "NPCActions")
	void Stop();	

private:

	static const FName targetKeyName;

	UPROPERTY(VisibleAnywhere, Category = "AIData")
	UBlackboardComponent* blackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = "AIData")
	UBlackboardData* npcBB;

	UPROPERTY(EditDefaultsOnly, Category = "AIData")
	UBehaviorTree* followTree;

	UPROPERTY(EditDefaultsOnly, Category = "AIData")
	UBehaviorTree* patrolTree;

};
