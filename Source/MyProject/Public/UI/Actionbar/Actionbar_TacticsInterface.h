// Created 1/15/21 9:0 PM

#pragma once
#include "UserWidget.h"
#include "Actionbar_TacticsInterface.generated.h"

class UButton;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTacticsPicked, int);

/**
 * Eventually there will be a tactics component which will have data for different strategies
 * we can tell groups of units to behave in a certain way. If there is ever multiplayer, there should
 * also be functionality for "tactic requests" in which a player can request that part of their units
 * be joined with another players squad. The other play would have to "accept" the tactics request.
 * TODO: COME BACK TO THIS
 */
UCLASS()
class MYPROJECT_API UActionbar_TacticsInterface : public UUserWidget
{
   GENERATED_BODY()

public:
   FOnTacticsPicked& OnTacticsPicked() { return OnTacticsPickedEvent; }

protected:
   UPROPERTY(meta=(BindWidget))
   UButton* Btn_Split;

   UPROPERTY(meta=(BindWidget))
   UButton* Btn_Surround;

   UPROPERTY(meta=(BindWidget))
   UButton* Btn_PositionalGroup;

   UPROPERTY(meta=(BindWidget))
   UButton* Btn_Lure;

   UPROPERTY(meta=(BindWidget))
   UButton* Btn_Escape;

   UPROPERTY(meta=(BindWidget))
   UButton* Btn_Run;

   UPROPERTY(meta=(BindWidget))
   UButton* Btn_Protect;

   UPROPERTY(meta=(BindWidget))
   UButton* Btn_Tank;

   UPROPERTY(meta=(BindWidget))
   UButton* Btn_Explore;

private:
   FOnTacticsPicked OnTacticsPickedEvent;            
};
