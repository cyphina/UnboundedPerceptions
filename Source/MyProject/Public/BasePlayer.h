// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "GameplayTagContainer.h"
#include "BasePlayer.generated.h"

/**
 * 	Class for data specific to the player
 */

class AUnit;
class AAlly;
class ASummon;
class ABaseHero;
class APickup;
class UQuestManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogTopicLearned, FGameplayTag, dialogTopic);

UCLASS()
class MYPROJECT_API ABasePlayer : public APlayerState
{
	GENERATED_BODY()

	/**List of what the player knows what to talk about*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Meta = (AllowPrivateAccess = true), Category = "Dialog")
	FGameplayTagContainer		dialogTopics;
	
	void						BeginPlay() override;
	virtual void				OnConstruction(const FTransform& transform) override;
	
public:
#pragma region PartyInfo
	/**Party leader should always be at slot 0*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
	TArray<ABaseHero*>			heroes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
	TArray<ABaseHero*>			selectedHeroes;

	/**Returns list of alive heroes and friendly units.  Heroes should be first in the list*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
	TArray<AAlly*>				allies; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
	TArray<AAlly*>				selectedAllies;
	
	/*List of all units summoned*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
	TArray<ASummon*>			summons;

	/*List of NPCs that joined the party (usually on escort missions)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
	TArray<AAlly*>				npcs;

#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
	AUnit*						focusedUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
	int							unitIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Quest")
	UQuestManager*				questManager;

	UPROPERTY(EditAnywhere)
	int							money;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
	FOnDialogTopicLearned		OnDialogLearned;

	ABasePlayer();

	/**Update the coins
	@param amount - this is the amount to update the coins by, and can be positive or negative
	*/
	UFUNCTION(BlueprintCallable, Category = "Player")
	void						UpdateGold(int32 amount);

	/**Update the exp gained by every party member
	@param amount - this is the amount every party member gains as exp. Can only be positive.
	*/
	UFUNCTION(BlueprintCallable, Category = "Player")
	void						UpdateEXP(int32 amount);

	/**When Cyphina or whoever is the MC at the moment learns of something new to talk about
	 *@param topic - This is the new conversation topic learned by MC
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogAccessor")
	void						LearnDialogTopic(FGameplayTag topic);

	UFUNCTION(BlueprintCallable, Category = "DialogAccessor")
	FGameplayTagContainer		GetDialogTopics() const { return dialogTopics; }
};
