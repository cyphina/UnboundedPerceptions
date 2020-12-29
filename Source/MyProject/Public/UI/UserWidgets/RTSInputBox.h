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

   UPROPERTY()
   class AHUDManager* hudManagerRef;

 public:
   UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
   FString txtInput;

   void Confirm() override;

   /** Look at my notes why accessors are important even in this seemingly trivial case. In particular, it's important for providing a debugging point. */
   FOnInputConfirmed& OnInputConfirmed() const { return OnInputConfirmedEvent; };

private:
   mutable FOnInputConfirmed OnInputConfirmedEvent;
};
