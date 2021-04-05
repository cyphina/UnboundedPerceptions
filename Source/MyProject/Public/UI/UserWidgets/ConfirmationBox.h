// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/PopupWidget.h"
#include "ConfirmationBox.generated.h"

/**
 * Confirmation box that appears when we want to let the player confirm an action
 */

DECLARE_DELEGATE_RetVal(bool, FOnConfirmation);

class AUserInput;

UCLASS()
class MYPROJECT_API UConfirmationBox : public UPopupWidget
{
   GENERATED_BODY()

   UPROPERTY()
   class AHUDManager* hudManagerRef;

<<<<<<< HEAD
 public:
   void            Confirm() override;
   FOnConfirmation& OnConfirmationMade() const { return OnConfirmationMadeEvent; }

=======
public:
   void Confirm() override;

   FOnConfirmation& OnConfirmationMade() const { return OnConfirmationMadeEvent; }

   void SetOnConfirmationMade(const FOnConfirmation& confirmationEvent) { OnConfirmationMadeEvent = confirmationEvent; }

>>>>>>> componentrefactor
private:
   mutable FOnConfirmation OnConfirmationMadeEvent;
};
