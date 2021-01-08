// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "GameplayTagContainer.h"
#include "BasePlayer.generated.h"

class AUnit;
class AAlly;
class ASummon;
class ABaseHero;
class APickup;
class UQuestManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogTopicLearned, FGameplayTag, dialogTopic);
DECLARE_EVENT(ABasePlayer, OnPartyUpdated);

/**
 * Class for data specific to the player that everyone needs to know.
 * Holds information that is replicated amongst clients about other clients
 */
UCLASS()
class MYPROJECT_API ABasePlayer : public APlayerState
{
   GENERATED_BODY()

   /**List of what the player knows what to talk about*/
   UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Meta = (AllowPrivateAccess = true), Category = "Dialog")
   FGameplayTagContainer dialogTopics = FGameplayTagContainer();

   void BeginPlay() override;

 public:
   static const int MAX_NUM_HEROES = 4;

   const TArray<ABaseHero*>& GetHeroes() const { return heroes; }

   AUnit* GetFocusedUnit() const { return focusedUnit; }
   void   SetFocusedUnit(AUnit* newFocusedUnit);

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
   TArray<ABaseHero*> selectedHeroes;

   /**List of every hero in the game *discovered currently* that may not be in the party currently*/
   UPROPERTY(BlueprintReadOnly, Category = "Party")
   TArray<ABaseHero*> allHeroes;

   /**
    * If there's any hero that is interacting currently with something blocking (e.g., storage or dialog)
    */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
   ABaseHero* heroInBlockingInteraction;

   /**Returns list of ALIVE heroes and friendly units.*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
   TArray<AAlly*> allies;

   /**List of all alive selected allies*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
   TArray<AAlly*> selectedAllies;

   /*List of all units summoned*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
   TArray<ASummon*> summons;

   /*List of NPCs that joined the party (usually on escort missions) */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
   TArray<AAlly*> npcs;

   /** Callback when we learn a new dialog topic*/
   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnDialogTopicLearned OnDialogLearned;

   /** Callback when we change the heroes in our party*/
   OnPartyUpdated partyUpdatedEvent;

   ABasePlayer();

   /** Helper function when all selected ally units are cleared */
   UFUNCTION(BlueprintCallable, Category = "Player Unit Management")
   void ClearSelectedAllies();

   /**
    *Change a party around
    *@param newHeroes - This is an array with the new heroes that will be in the party.  Must be of sizes 1-4
    */
   UFUNCTION(BlueprintCallable, Category = "Player Unit Management")
   void UpdateParty(TArray<ABaseHero*> newHeroes);

   /**Called when a new hero joins the team and can be assigned to the 4 man squad*/
   UFUNCTION(BlueprintCallable, Category = "Player Unit Management")
   void JoinParty(ABaseHero* newHero);

   /**
   *Update the coins
   *@param amount - this is the amount to update the coins by, and can be positive or negative
   */
   UFUNCTION(BlueprintCallable, Category = "Player")
   void UpdateGold(int32 amount);

   /**
   *Update the exp gained by every party member
   *@param amount - this is the amount every party member gains as exp. Can only be positive.
   */
   UFUNCTION(BlueprintCallable, Category = "Player")
   void UpdateEXP(int32 amount);

   /**
    *When Cyphina or whoever is the MC at the moment learns of something new to talk about
    *@param topic - This is the new conversation topic learned by MC
    */
   UFUNCTION(BlueprintCallable, Category = "DialogAccessor")
   void LearnDialogTopic(FGameplayTag topic);

   /** Retrieves a list of all dialog topics this player knows */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogAccessor")
   const FGameplayTagContainer& GetDialogTopics() const { return dialogTopics; }

   /**Check to see if the player has learned this topic */
   UFUNCTION(BlueprintCallable, Category = "DialogAccessor")
   FORCEINLINE bool HasDialogTopic(FGameplayTag tagToCheck) const { return dialogTopics.HasTagExact(tagToCheck); }

   UFUNCTION(BlueprintCallable)
   int GetMoney() const { return money; }

   UFUNCTION(BlueprintCallable)
   void SetMoney(int newMoneyVal) { money = newMoneyVal; }

 protected:
   /**
    * List of active heroes.
    * Party leader should always be at slot 0.
    */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
   TArray<ABaseHero*> heroes;

   /** Enemy or hero unit that we see detailed information in our actionbar */
   AUnit* focusedUnit = nullptr;

 private:
   void OnHeroSelected(ABaseHero* heroRef);
   void OnAllySelected(AAlly* allyRef);
   void OnUnitSelected(AUnit* unitRef);
   void OnHeroDeselected(ABaseHero* heroRef);
   void OnAllyDeselected(AAlly* allyRef);
   void OnUnitDeselected(AUnit* unitRef);

   void OnAllyActiveChanged(AAlly* allyRef, bool isActive);
   void OnHeroActiveChanged(ABaseHero* heroRef, bool isActive);
   void OnSummonActiveChanged(ASummon* summonRef, bool isActive);

   /** How much squeezies we have (that's the currency name... for now) */
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
   int money;
};
