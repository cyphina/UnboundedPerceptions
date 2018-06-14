#include "MyProject.h"
#include "RTSDoor.h"
#include "RTSGameMode.h"
#include "EventSystem/RTSConditional.h"
#include "UserInput.h"
#include "HUDManager.h"
#include "WorldObjects/BaseHero.h"


// Sets default values
ARTSDoor::ARTSDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(scene);
	doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	doorMesh->SetupAttachment(RootComponent);

	doorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollision"));
	doorCollision->SetupAttachment(RootComponent);
	doorCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3); //Interactable
}

// Called when the game starts or when spawned
void ARTSDoor::BeginPlay()
{
	Super::BeginPlay();
	//checking if the curvefloat that was entered in the blueprints was valid
	if (progressCurve)
	{
		/* ProgressFunction contains signature of the function that is going to execute very tick.  It's a delegate*/
		FOnTimelineFloat progressFunction;

		/* In order to bind the function our ufunction, we need a name its name */
		progressFunction.BindUFunction(this, FName("HandleProgress"));

		//Setup the loop status and function that is going to fire when the timeline ticks
		tL.AddInterpFloat(progressCurve, progressFunction);
		tL.SetLooping(false);
		tL.SetTimelineLength(1.f);
	}
	initialRotation = doorMesh->GetComponentRotation();
	finalRotation = initialRotation + FRotator(0, 90, 0);

	gameModeRef = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
	cpcRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
}

// Called every frame
void ARTSDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	tL.TickTimeline(DeltaTime);
}

void ARTSDoor::Interact_Implementation(ABaseHero* hero)
{
	//Put code here that places the item in the characters inventory
	if (CanInteract_Implementation())
	{
		if (!isOpen)
		{
			tL.Play();
			//SetActorEnableCollision(false);
			doorCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore); //Friendly
			doorCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore); //Enemy
			doorCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore); //Friendly
		}
		else
		{
			tL.Reverse();
			doorCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block); //Friendly
			doorCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block); //Enemy
			doorCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Block); //Friendly

		}
		isOpen = !isOpen;
	}
	else
	{
		TArray<FDialogData> dialogData;
		dialogData.Emplace(TArray<int>(), gameModeRef->GetConditionalManager()->GetConditionString(useConditions), *hero->GetGameName().ToString());
		cpcRef->GetHUDManager()->AddHUDDialogString(MoveTemp(dialogData));
	}
}

FVector ARTSDoor::GetInteractableLocation_Implementation()
{
	return GetActorLocation();
}

bool ARTSDoor::CanInteract_Implementation()
{
	for (FConditionData condition : useConditions)
	{
		if (!gameModeRef->GetConditionalManager()->GetCondition(condition))
		{
			return false;
		}
	}
	return true;
}

void ARTSDoor::HandleProgress(float value)
{
	FRotator newRotation = FMath::Lerp(initialRotation, finalRotation, value);
	doorMesh->SetRelativeRotation(newRotation);
}
