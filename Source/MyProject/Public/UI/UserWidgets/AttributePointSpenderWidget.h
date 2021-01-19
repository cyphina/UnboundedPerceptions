#pragma once

#include "StatEnums.h"
#include "Blueprint/UserWidget.h"
#include "AttributePointSpenderWidget.generated.h"

class UTextBlock;
class UButton;

/**
 * Allows us to allocate points towards improving a hero's attribute.
 */
UCLASS()
class MYPROJECT_API UAttributePointSpenderWidget : public UUserWidget
{
   GENERATED_BODY()

public:
   static UAttributePointSpenderWidget* MakePointSpenderWidget(UUserWidget* widgetParent, TSubclassOf<UAttributePointSpenderWidget> widgetClass, EAttributes attCategory);

protected:
   void NativeOnInitialized() override;
   
   UPROPERTY(Meta=(BindWidget))
   UButton* Btn_Decrement;

   UPROPERTY(Meta=(BindWidget))
   UButton* Btn_Increment;

   UPROPERTY(Meta=(BindWidget))
   UTextBlock* Text_Category;

   UPROPERTY(Meta=(BindWidget))
   UTextBlock* Text_Value;
   
private:
   void OnAttributeIncreased();
   
   void OnAttributeDecreased();
   
   EAttributes attCategory;
};
