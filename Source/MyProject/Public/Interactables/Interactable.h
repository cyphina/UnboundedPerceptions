#pragma once
#include "Interactable.generated.h"

/**
 * Classes derived from interfaces use no extra memory because of empty base optimization
 */

class ABaseHero;

/**Use this uobject to expose the interface to blueprints*/
UINTERFACE(BlueprintType, MinimalAPI)
class MYPROJECT_API UInteractable : public UInterface
{
   GENERATED_UINTERFACE_BODY()
};

class MYPROJECT_API IInteractable
{
   GENERATED_IINTERFACE_BODY()

 public:
   /** classes using this interface must implement this function
    * @param hero - The hero who interacted with this interactable
    */
   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractDetails")
   void Interact(ABaseHero* hero);

   /** Classes using this interface must implement this function */
   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractDetails")
   FVector GetInteractableLocation() const;

   /**Can this interactable be used?*/
   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractDetails")
   bool CanInteract() const;
};
