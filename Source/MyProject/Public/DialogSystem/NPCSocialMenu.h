// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "NPCSocialMenu.generated.h"

class ANPC;
class AHUDManager;
class UButton;

/**
 * Dialog widget for talking to NPCs, and IntimateNPCs,
 */

/**Current view for our social widget*/
UENUM(BlueprintType)
enum class ESocialHUDState : uint8 {
   /**View when talking to a random*/
   Conversation,
   /**View when talking to someone who you can gain relationship points*/
   Intimate,
   /**View when talking to a shopkeeper*/
   Shop
};

UCLASS()
class MYPROJECT_API UNPCSocialMenu : public UMyUserWidget
{
   GENERATED_BODY()

   UPROPERTY(BlueprintReadOnly, Category = "References", Meta = (AllowPrivateAccess = true))
   ANPC* npcRef = nullptr;

   UPROPERTY()
   AHUDManager* hudManagerRef;

   /**Storage of the current view since after we press talk, we hide the view and eventually will need to get back to it.*/
   ESocialHUDState socialHUDState;

   /** Collapses (bHide == true) or shows all butttons*/
   void HideOrShowAllButtons(bool bHide=true) const;

 protected:
   static const int NUM_BUTTONS = 5;

   UPROPERTY(BlueprintReadWrite, Category = "SocialOptions", Meta = (BindWidget))
   UButton* btnTalk;
   UPROPERTY(BlueprintReadWrite, Category = "SocialOptions", Meta = (BindWidget))
   UButton* btnGift;
   UPROPERTY(BlueprintReadWrite, Category = "SocialOptions", Meta = (BindWidget))
   UButton* btnSharedPsychosis;
   UPROPERTY(BlueprintReadWrite, Category = "SocialOptions", Meta = (BindWidget))
   UButton* btnShop;
   UPROPERTY(BlueprintReadWrite, Category = "SocialOptions", Meta = (BindWidget))
   UButton* btnLeave;

   UFUNCTION()
   void Talk();

   UFUNCTION()
   void Gift();

   UFUNCTION()
   void Psychosis();

   UFUNCTION()
   void Shop();

   UFUNCTION()
   void Leave();

 public:
   void NativeConstruct() override;

   UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget))
   class UDialogWheel* dialogWheel;

   /** Called after greeting the IntimateNPC */
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Callbacks")
   void PostGreeting();

   /** View for NPCs which you can't gain friends from, but you can ask about topics from */
   void SetConversationView();

   /** View for NPCs which we can gain affinity with*/
   void SetIntimateView();

   /** View for salespeople NPC*/
   void SetShopView();

   UFUNCTION(BlueprintCallable, Category = "Callbacks")
   void SetMainView();

   bool  OnWidgetAddToViewport_Implementation() override final;
   void  SetNPC(ANPC* newNpcRef) { this->npcRef = newNpcRef; }
   ANPC* GetNPC() const { return npcRef; }
};
