#pragma once

#include "GameFramework/Actor.h"
#include "Searchable.generated.h"

/*Interactable we can scavange after a delay searching */
UCLASS()
class MYPROJECT_API ASearchable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASearchable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
