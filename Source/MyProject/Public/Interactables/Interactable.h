#pragma once
#include "Interactable.generated.h"

/**
 * Classes derived from interfaces use no extra memory because of empty base optimization
 */

class ABaseHero;

/**
 * @brief UObject corresponding to interface which holds behaviors for all objects that can be interacted with.
 */
UINTERFACE(BlueprintType, MinimalAPI)
class UInteractable : public UInterface
{
   GENERATED_UINTERFACE_BODY()
};

class IInteractable
{
   GENERATED_IINTERFACE_BODY()

 public:
   /**
    * Action that occurs when a player interacts with this interactable by clicking on it
    * @param hero - The hero who interacted with this interactable
    */
   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractDetails")
   void         Interact(ABaseHero* hero);

   /** Classes using this interface must implement this function */
   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractDetails")
   FVector         GetInteractableLocation() const;

   /**Can this interactable be used?*/
   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractDetails")
   bool         CanInteract() const;
};
