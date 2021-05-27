// Created 8/30/20 11:18 AM

#pragma once
#include "UserWidget.h"
#include "GameSpeedWidget.generated.h"

class UButton;
class UTextBlock;
class ARTSGameState;

/** Displays the game speed stored in the Game State and can also modify it.
  * The speed should not affect anything besides the main game, and serves as a way to help people level up
  * or make fights easier.
  * In multiplayer this should be disabled.
  */
UCLASS()
class MYPROJECT_API UGameSpeedWidget : public UUserWidget
{
   GENERATED_BODY()

 protected:
   void NativeOnInitialized() override;

 private:
   UFUNCTION()
   void IncreaseGameSpeed();

   UFUNCTION()
   void DecreaseGameSpeed();

   UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
   ARTSGameState* gameStateRef;

   UPROPERTY(meta = (BindWidget))
   UTextBlock* Text_GameSpeed;

   UPROPERTY(meta = (BindWidget))
   UButton* Btn_IncGameSpeed;

   UPROPERTY(meta = (BindWidget))
   UButton* Btn_DecGameSpeed;

   /**Used to index what speed to go at in our predefined game speeds*/
   UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
   int speedIndex = 2;

   /** Array with possible speed multipliers the game can run off of */
   const TArray<float, TFixedAllocator<5>> gameSpeeds = {0, 0.5, 1, 1.5, 2, 5};
};
