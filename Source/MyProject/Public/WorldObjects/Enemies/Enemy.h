// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WorldObjects/Unit.h"
#include "Enemy.generated.h"

/**
 * Base class for all enemy units
 */

class AUserInput;
class UMyItem;
class ARTSGameMode;

USTRUCT(BlueprintType, NoExport)
struct FSpellCombination
{
	TArray<TSubclassOf<UMySpell*>>			combination; //spells to be used in tandem
	TArray<int>								delay; //delay between two actions
	TArray<int>								vulnerabilityThreshold; //continue with action if target passes this threshold
};

UCLASS()
class MYPROJECT_API AEnemy : public AUnit
{
	GENERATED_BODY()
	/**
	 *range enemy will attack you
	 */
	int										aggroRange;
	
	/**
	 *Counter for number of enemies (units with opposite value of isEnemy) that can see this unit
	 */
	int										enemyVisionCount;

public:
	AEnemy(const FObjectInitializer& oI);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int										health;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int										mana;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int										expGiven; //how much money given on death
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int										moneyGiven; //how much exp given on death
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly) //List of combinations by priority.  Combination 0 will always be an opener
	TArray<FSpellCombination>				combinations;

/*--Callbacks--*/
#pragma region callbacks
	void									BeginPlay() override;
	//Enemy tick behavior will be controlled by AI
	void									Tick(float deltaSeconds) override;
	void									Die() override;
#pragma endregion

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	void									SetSelected(bool value) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Vision")
	int								 GetVisionCount() const { return enemyVisionCount; }
	UFUNCTION(BlueprintCallable, Category = "Vision")
	void							 IncVisionCount() { ++enemyVisionCount; }
	UFUNCTION(BlueprintCallable, Category = "Vision")
	void							 DecVisionCount() { --enemyVisionCount;}

private:

	AUserInput* controllerRef;
	ARTSGameMode* gameModeRef;
};
