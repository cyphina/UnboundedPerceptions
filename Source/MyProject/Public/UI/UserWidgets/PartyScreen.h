#pragma once

#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "PartyScreen.generated.h"

class UButton;
class UTextBlock;
/**
 * Screen used to swap out party members.
 */
UCLASS()
class MYPROJECT_API UPartyScreen : public UMyUserWidget
{
   GENERATED_BODY()

 public:
   virtual bool OnWidgetAddToViewport_Implementation() override;

 protected:
   UPROPERTY(Meta = (BindWidget))
   UButton* Btn_PartyMember1;

   UPROPERTY(Meta = (BindWidget))
   UButton* Btn_PartyMember2;

   UPROPERTY(Meta = (BindWidget))
   UButton* Btn_PartyMember3;

   UPROPERTY(Meta = (BindWidget))
   UButton* Btn_PartyMember4;

   UPROPERTY(Meta = (BindWidget))
   UTextBlock* Text_PartyMemberName1;

   UPROPERTY(Meta = (BindWidget))
   UTextBlock* Text_PartyMemberName2;

   UPROPERTY(Meta = (BindWidget))
   UTextBlock* Text_PartyMemberName3;

   UPROPERTY(Meta = (BindWidget))
   UTextBlock* Text_PartyMemberName4;
};
