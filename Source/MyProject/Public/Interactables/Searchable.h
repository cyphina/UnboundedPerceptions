#pragma once

#include "GameFramework/Actor.h"
#include "Items/Item.h"
#include "Searchable.generated.h"

/*Interactable we can scavange after a delay searching (think of those things in escape room games) 
 * Examples: Treasure chests, table, shelf, and cupboard.  
 */
UCLASS()
class MYPROJECT_API ASearchable : public AActor
{
	GENERATED_BODY()
	
public:	
	ASearchable();

	/**Items that can be looted from this place*/
	UPROPERTY(EditAnywhere)
	TArray<UMyItem*>			lootableItems;

	UPROPERTY(EditAnywhere)
	bool					isLocked;


protected:
	virtual void			BeginPlay() override;

public:	
	virtual void			Tick(float DeltaTime) override;

};
