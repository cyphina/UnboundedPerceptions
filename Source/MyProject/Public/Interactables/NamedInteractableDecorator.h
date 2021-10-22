#pragma once
#include "GameFramework/Actor.h"
#include "Interactables/InteractableActorDecoratorBase.h"
#include "NamedInteractableDecorator.generated.h"

/**
 * Decorator that allows us to give a name property to an interactable
 */

UCLASS()
class MYPROJECT_API UNamedInteractableDecorator : public UInteractableActorDecoratorBase
{
   GENERATED_BODY()

public:
   FORCEINLINE FText GetName() const override { return name; }

protected:
   /**Used to identify interactables in quests*/
   UPROPERTY(EditAnywhere, Category = "Interactable Settings")
   FText name;

   bool Interact() override final;
};
