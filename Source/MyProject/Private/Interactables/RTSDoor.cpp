#include "MyProject.h"
#include "RTSDoor.h"
#include "RTSGameMode.h"
#include "EventSystem/RTSConditional.h"
#include "UserInput.h"
#include "HUDManager.h"

#include "Interactables/ConditionalInteractableDecorator.h"
#include "Interactables/TriggerInteractableDecorator.h"

#include "WorldObjects/BaseHero.h"
#include "LevelSaveStructs.h"


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

	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Game/RTS_Tutorial/HUDs/ActionUI/StandardLinear"));
	if(curve.Succeeded())
		progressCurve = curve.Object;
}

// Called when the game starts or when spawned
void ARTSDoor::BeginPlay()
{
	UConditionalInteractableDecorator* dec = NewObject<UConditionalInteractableDecorator>(this,"DoorLockedCndition");
	FConditionData condition = FConditionData();
	condition.conditionalType = EConditionalType::CustomCond;
	condition.conditionalValues = TArray<FString>({FString::FromInt(!isLocked)});
	dec->conditions.Add(condition);
	dec->customDialogConversation = "ConditionalDoorLocked";
	decorator = dec;

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
}

FVector ARTSDoor::GetInteractableLocation_Implementation()
{
	return GetActorLocation();
}

bool ARTSDoor::CanInteract_Implementation()
{
	return Super::CanInteract_Implementation() && !isLocked;
}

void ARTSDoor::HandleProgress(float value)
{
	FRotator newRotation = FMath::Lerp(initialRotation, finalRotation, value);
	doorMesh->SetRelativeRotation(newRotation);
}

void ARTSDoor::SaveInteractable(FMapSaveInfo& mapData)
{
	FDoorInteractableSaveInfo doorSaveInfo;
	UInteractableActorDecoratorBase* decor = decorator;

	doorSaveInfo.isLocked = isLocked;
	doorSaveInfo.interactableInfo = SaveInteractableData();
}

void ARTSDoor::LoadInteractable(FDoorInteractableSaveInfo& doorData)
{
	AInteractableBase::LoadInteractableData(doorData.interactableInfo);
	isLocked = doorData.isLocked;
}
