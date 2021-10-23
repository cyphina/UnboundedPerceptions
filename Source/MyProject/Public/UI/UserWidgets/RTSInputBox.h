#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/PopupWidget.h"
#include "RTSInputBox.generated.h"

DECLARE_DELEGATE_RetVal_OneParam(bool, FOnInputConfirmed, FString);

/**
 * Class that represents a box that can take in input and perform some function using it
 */
UCLASS()
class MYPROJECT_API URTSInputBox : public UPopupWidget
{
   GENERATED_BODY()

 public:
   UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
   FString txtInput;

   void Confirm() override;

   FOnInputConfirmed& OnInputConfirmed() const { return OnInputConfirmedEvent; };

   void SetOnInputConfirmed(const FOnInputConfirmed& confirmationEvent) { OnInputConfirmedEvent = confirmationEvent; }
   
private:
   UPROPERTY()
   class AHUDManager* hudManagerRef;
   
   mutable FOnInputConfirmed OnInputConfirmedEvent;
};
