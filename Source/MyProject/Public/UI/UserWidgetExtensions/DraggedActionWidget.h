#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DraggedActionWidget.generated.h"

class UImage;
/**
 * Widget that follows our cursor as we drag things around
 */
UCLASS()
class MYPROJECT_API UDraggedActionWidget : public UUserWidget
{
   GENERATED_BODY()

 public:
   void SetDraggedImage(UTexture2D* newImage);

 protected:
   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
   UImage* Image_Item;
};
