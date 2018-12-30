#include "MyProject.h"
#include "RTSDoor.h"
#include "EventSystem/RTSConditional.h"
#include "HUDManager.h"
#include "RTSGameMode.h"
#include "UserInput.h"

#include "Interactables/ConditionalInteractableDecorator.h"
#include "Interactables/TriggerInteractableDecorator.h"

#include "LevelSaveStructs.h"
#include "WorldObjects/BaseHero.h"

// Sets default values
ARTSDoor::ARTSDoor() : AInteractableBase()
{
   PrimaryActorTick.bCanEverTick = true;

   doorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollision"));
   doorCollision->SetupAttachment(RootComponent);
   doorCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3); // Interactable

   doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("DoorMesh"));
   doorMesh->SetCollisionProfileName("NoCollision");
   doorMesh->SetMobility(EComponentMobility::Movable);
   doorMesh->SetupAttachment(RootComponent);

   ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Game/RTS_Tutorial/HUDs/ActionUI/StandardLinear"));
   if (curve.Succeeded()) progressCurve = curve.Object;
}

// Called when the game starts or when spawned
void ARTSDoor::BeginPlay()
{
   UConditionalInteractableDecorator* dec       = NewObject<UConditionalInteractableDecorator>(this, "DoorLockedCndition");
   FConditionData                     condition = FConditionData();
   condition.conditionalType                    = EConditionalType::CustomCond;
   condition.conditionalValues                  = TArray<FString>({FString::FromInt(!isLocked)});
   dec->conditions.Add(condition);
   dec->customDialogConversation = "ConditionalDoorLocked";
   decorator                     = dec;

   Super::BeginPlay();

   // checking if the curvefloat that was entered in the blueprints was valid
   if (progressCurve) {
      /* ProgressFunction contains signature of the function that is going to execute very tick.  It's a delegate*/
      FOnTimelineFloat progressFunction;

      /* In order to bind the function our ufunction, we need a name its name */
      progressFunction.BindUFunction(this, FName("HandleProgress"));

      // Setup the loop status and function that is going to fire when the timeline ticks
      tL.AddInterpFloat(progressCurve, progressFunction);
      tL.SetLooping(false);
      tL.SetTimelineLength(1.f);
   }

   initialRotation = FRotator(0, 0, 0);
   finalRotation   = FRotator(0, 90, 0);
}

// Called every frame
void ARTSDoor::Tick(float DeltaTime)
{
   //I guess this means the door won't open until it is on the screen so if we try and do any longrange opening...  it's not going to work till we see it
   if (WasRecentlyRendered())
   {
      Super::Tick(DeltaTime);
      tL.TickTimeline(DeltaTime);
   }
}

void ARTSDoor::Interact_Implementation(ABaseHero* hero)
{
   // Put code here that places the item in the characters inventory
   if (CanInteract_Implementation()) {
      if (!isOpen) {
         tL.Play();
         // SetActorEnableCollision(false);
         doorCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);              // Friendly
         doorCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore); // Enemy
         doorCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore); // Friendly
      } else {
         tL.Reverse();
         doorCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);              // Friendly
         doorCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block); // Enemy
         doorCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Block); // Friendly
      }
      isOpen = !isOpen;
   }
}

FVector ARTSDoor::GetInteractableLocation_Implementation(ABaseHero* hero)
{
   // Figure out what side of the door we are on by checking the coordinates of the actor relative to the door
   FMatrix2x2 changeOfBasisM = FMatrix2x2(GetActorRightVector().X, GetActorForwardVector().X, GetActorRightVector().Y, GetActorForwardVector().Y);
   changeOfBasisM.Inverse();
   FVector2D heroLocation   = FVector2D(hero->GetActorLocation() - GetActorLocation());
   FVector2D heroDoorCoords = changeOfBasisM.TransformVector(heroLocation);
   GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, heroDoorCoords.ToString());
   if (heroDoorCoords.Y > 0)
      return GetActorForwardVector() * ABaseHero::interactRange + GetActorLocation();
   else
      return GetActorForwardVector() * ABaseHero::interactRange * -1 + GetActorLocation();
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
   FDoorInteractableSaveInfo        doorSaveInfo;
   UInteractableActorDecoratorBase* decor = decorator;

   doorSaveInfo.isLocked         = isLocked;
   doorSaveInfo.interactableInfo = SaveInteractableData();
}

void ARTSDoor::LoadInteractable(FDoorInteractableSaveInfo& doorData)
{
   AInteractableBase::LoadInteractableData(doorData.interactableInfo);
   isLocked = doorData.isLocked;
}
