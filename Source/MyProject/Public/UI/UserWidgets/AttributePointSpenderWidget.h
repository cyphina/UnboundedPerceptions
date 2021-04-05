#pragma once

#include "StatEnums.h"
#include "Blueprint/UserWidget.h"
#include "AttributePointSpenderWidget.generated.h"

class UCharacterMenu;
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

   EAttributes GetAttCategory() const { return attCategory; }

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
   UCharacterMenu* parentWidget;

   UFUNCTION()
   FText GetAttributeValues();

   UFUNCTION()
   void OnAttributeIncreased();

   UFUNCTION()
   void OnAttributeDecreased();

   EAttributes attCategory;
};
