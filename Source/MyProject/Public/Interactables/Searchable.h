#pragma once

#include "Interactables/InteractableBase.h"
#include "Items/Item.h"
#include "Searchable.generated.h"

/*Interactable we can scavenge after a delay searching (think of those things in escape room games)
 * Examples: Treasure chests, table, shelf, and cupboard.
 */
UCLASS()
class MYPROJECT_API ASearchable : public AInteractableBase
{
   GENERATED_BODY()

 public:
   ASearchable();

   /**Items that can be looted from this place*/
   UPROPERTY(EditAnywhere)
   TArray<FMyItem> lootableItems;

   UPROPERTY(EditAnywhere)
   bool isLocked;

 protected:
   virtual void BeginPlay() override;

 public:
   virtual void Tick(float DeltaTime) override;
};
