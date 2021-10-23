#pragma once

#include "Blueprint/UserWidget.h"
#include "DragDropOperation.h"
#include "MyUserWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMyWidgetVisibilityChanged, ESlateVisibility, NewVisibility);

class AUserInput;

/**
 * Adds some extra lightweight functionality to the basic widget class.
 * TODO: Split out the widgets that are stored in the hudmanager to be a subclass of this widget so that we can have primitives deriving from this one.
 */

UCLASS(abstract)
class MYPROJECT_API UMyUserWidget : public UUserWidget
{
   GENERATED_BODY()

   friend class AHUDManager;
   
public:
   /**
    * Copycat of Svelte event dispatcher.
    * Requires us to have event handlers that are UFUNCTION() with the name On<EventName>
    */
   void DispatchEvent(FName eventName);

   /** Overriden to allow for callbacks when our visiblity is switched */
   void SetVisibility(ESlateVisibility newVisibility) override;

   /**
   * @brief Callback whenever toggled on screen
   * @return Returns true if it successfully was added the widget to screen
   */
   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Callback")
   bool         OnWidgetAddToViewport();
   virtual bool OnWidgetAddToViewport_Implementation();

   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Callback")
   void         OnWidgetRemovedFromViewport();
   virtual void OnWidgetRemovedFromViewport_Implementation();
   
protected:
   /**
    * I used to think Construct ran before NativeConstruct(), but I didn't realize that calling the Super function of NativeConstruct called Construct() and I usually
    * called the parent class NativeConstruct first so I thought it went before >_<
    */
   void NativeConstruct() override;

   UFUNCTION(BlueprintNativeEvent, Category = "Callbacks")
   void OnMyWidgetVisibilityChanged(ESlateVisibility newVisibility);
   virtual void OnMyWidgetVisibilityChanged_Implementation(ESlateVisibility newVisibility) {}

   UPROPERTY(BlueprintReadWrite, Category = "References")
   AUserInput* CPC;

private:
};
