#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableActorDecoratorBase.h"
#include "DEPRECATED_RTSConditional.h"
#include "ConditionalInteractableDecorator.generated.h"

/**
 * Decorator for adding condition when trying to use a interactable.
 */

class UConditionalManager;
class ARTSGameMode;

UCLASS()
class MYPROJECT_API UConditionalInteractableDecorator : public UInteractableActorDecoratorBase
{
   GENERATED_BODY()

 public:
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   TArray<FConditionData> conditions;

   /** Name of the custom conversation if conditions not met.  If empty, uses default notification (generated from conditions)*/
   UPROPERTY(EditAnywhere)
   FName customDialogConversation;

   void Init() override;

   /** If this returns false, the interactable won't trigger any logic when interacted with */
   bool Interact() override;
};
