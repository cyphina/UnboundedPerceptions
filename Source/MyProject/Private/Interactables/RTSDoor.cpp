#include "MyProject.h"
#include "RTSDoor.h"
#include "EventSystem/DEPRECATED_RTSConditional.h"
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
   doorCollision->SetCollisionObjectType(INTERACTABLE_CHANNEL);
   static ConstructorHelpers::FObjectFinder<UStaticMesh> doorFrameMesh(TEXT("/Game/RTS_Tutorial/Meshes/SM_DoorFrame"));
   // Caching the data table information can be problematic if we reimport
   if(doorFrameMesh.Object)
      interactableMesh->SetStaticMesh(doorFrameMesh.Object);

   doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("DoorMesh"));
   doorMesh->SetCollisionProfileName("NoCollision");
   doorMesh->SetMobility(EComponentMobility::Movable);

   // Attach door to the hinge
   static const FName DOOR_SOCKET_NAME = "Door";
   // Using SetupAttachment did not work when setting up the socket
   doorMesh->AttachToComponent(interactableMesh, FAttachmentTransformRules::KeepRelativeTransform, DOOR_SOCKET_NAME);

   ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Game/RTS_Tutorial/HUDs/ActionUI/StandardLinear"));
   if(curve.Succeeded())
      progressCurve = curve.Object;
}

// Called when the game starts or when spawned
void ARTSDoor::BeginPlay()
{
   // If our door is locked, create a new decorator (unless we already have one)
   if(isLocked) {
      auto dec = decorator;
      while(dec) {
         if(dec->GetClass()->IsChildOf(UConditionalInteractableDecorator::StaticClass()))
            break;
         dec = dec->decoratedInteractable;
      }

      FConditionData condition    = FConditionData();
      condition.conditionalType   = EConditionalType::CustomCond;
      condition.conditionalValues = TArray<FString>({FString::FromInt(!isLocked)});

      // If we don't already have a conditional decorator, create a new one
      if(!dec) {
         UConditionalInteractableDecorator* newCondDec = NewObject<UConditionalInteractableDecorator>(this, "DoorLockedCondition");
         newCondDec->conditions.Add(condition);
         newCondDec->customDialogConversation = "ConditionalDoorLocked";
         dec                                  = newCondDec;
      } else { // Else just append this as a new condition
         UConditionalInteractableDecorator* storedCondDec = Cast<UConditionalInteractableDecorator>(dec);
         storedCondDec->conditions.Add(condition);
         storedCondDec->customDialogConversation = "ConditionalDoorLocked";
      }
   }

   Super::BeginPlay();

   // Checking if the curvefloat that was assigned in blueprints was valid
   if(progressCurve) {
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

   cpcRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
}

// Called every frame
void ARTSDoor::Tick(float DeltaTime)
{
   // I guess this means the door won't open until it is on the screen so if we try and do any longrange opening...  it's not going to work till we see it
   if(WasRecentlyRendered()) {
      Super::Tick(DeltaTime);
      tL.TickTimeline(DeltaTime);
   }
}

void ARTSDoor::Interact_Implementation(ABaseHero* hero)
{
   // Put code here that places the item in the characters inventory
   if(CanInteract_Implementation()) {
      if(!isOpen) {
         // Open the Door
         tL.Play();
         // SetActorEnableCollision(false);
         doorCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
         doorCollision->SetCollisionResponseToChannel(ENEMY_OBJECT_CHANNEL, ECR_Ignore);
         doorCollision->SetCollisionResponseToChannel(ALLY_OBJECT_CHANNEL, ECR_Ignore);
      } else {
         // Close the Door
         tL.Reverse();
         doorCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
         doorCollision->SetCollisionResponseToChannel(ENEMY_OBJECT_CHANNEL, ECR_Block);
         doorCollision->SetCollisionResponseToChannel(ALLY_OBJECT_CHANNEL, ECR_Block);
      }
      isOpen = !isOpen;
   }
}

FVector ARTSDoor::GetInteractableLocation_Implementation() const
{
   // Figure out what side of the door we are on by checking the coordinates of the actor relative to the door
   ABaseHero* heroRef = nullptr;
   for(auto hero : cpcRef->GetBasePlayer()->GetHeroes()) {
      if(hero->GetCurrentInteractable() == this) {
         heroRef = hero;
         break;
      }
   }

   if(!heroRef) {
      UE_LOG(LogTemp, Error, TEXT("No interactable hero when moving to door named %s"), *GetName());
      return FVector();
   }

   FMatrix2x2 changeOfBasisM      = FMatrix2x2(GetActorRightVector().X, GetActorForwardVector().X, GetActorRightVector().Y, GetActorForwardVector().Y);
   changeOfBasisM                 = changeOfBasisM.Inverse();
   const FVector2D heroLocation   = FVector2D(heroRef->GetActorLocation() - GetActorLocation());
   const FVector2D heroDoorCoords = changeOfBasisM.TransformVector(heroLocation);
   GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, heroDoorCoords.ToString());
   if(heroDoorCoords.Y > 0)
      return GetActorForwardVector() * ABaseHero::INTERACT_RANGE + GetActorLocation();
   else
      return GetActorForwardVector() * ABaseHero::INTERACT_RANGE * -1 + GetActorLocation();
}

bool ARTSDoor::CanInteract_Implementation() const
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
   doorSaveInfo.interactableInfo = SaveInteractableData().interactableInfo;
   mapData.doorInteractables.Add(doorSaveInfo);
}

void ARTSDoor::LoadInteractable(FMapSaveInfo& mapData)
{
   FDoorInteractableSaveInfo* doorInteractableInfo = mapData.doorInteractables.FindByHash<ARTSDoor*>(GetTypeHash(*this), this);
   if(doorInteractableInfo) {
      LoadInteractableData(doorInteractableInfo->interactableInfo);
      isLocked = doorInteractableInfo->isLocked;
   }
}
