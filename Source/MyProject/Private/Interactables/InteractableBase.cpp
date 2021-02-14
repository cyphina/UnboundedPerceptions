#include "MyProject.h"
#include "InteractableBase.h"

#include "GameplayDelegateContext.h"
#include "Interactables/InteractableActorDecoratorBase.h"
#include "Interactables/NamedInteractableDecorator.h"
#include "TriggerInteractableDecorator.h"
#include "LevelSaveStructs.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
   PrimaryActorTick.bCanEverTick = false;

   sceneRoot = CreateDefaultSubobject<USceneComponent>(FName("Scene"));
   SetRootComponent(sceneRoot);

   interactableMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
   interactableMesh->SetCollisionProfileName("Interactable");

   sceneLocation = CreateDefaultSubobject<USceneComponent>(FName("InteractLocation"));

   interactableMesh->SetupAttachment(RootComponent);
   sceneLocation->SetupAttachment(RootComponent);
}

void AInteractableBase::BeginPlay()
{
   Super::BeginPlay();
   if (decorator) {
      // decorator->Rename(0,this);
      decorator->Init();
   }
}

FInteractableSaveInfoWrapper AInteractableBase::SaveInteractableData()
{
   FInteractableSaveInfoWrapper     interactableSaveInfo;
   UInteractableActorDecoratorBase* decor = decorator;

   // Save trigger decoration since it is the only decoration with modified state
   while (decor) {
      if (decor->GetClass() == UTriggerInteractableDecorator::StaticClass()) {
         UTriggerInteractableDecorator* triggerDecor = Cast<UTriggerInteractableDecorator>(decor);
         for (FTriggerData& finishedTriggerActivation : triggerDecor->triggersActivatedOnInteract) {
            interactableSaveInfo.interactableInfo.triggerDecoratorInfo.Add(FTriggerInteractableDecoratorSaveInfo(finishedTriggerActivation.numCalls, finishedTriggerActivation.enabled));
         }
         break;
      }
      decor = decor->decoratedInteractable;
   }

   // Save transform and class
   interactableSaveInfo.interactableInfo.transform         = GetTransform();
   interactableSaveInfo.interactableInfo.interactableClass = GetClass();

   return interactableSaveInfo;
}

void AInteractableBase::LoadInteractableData(FInteractableSaveInfo& interactableInfo)
{
   // Restore transform is moveable else we get warnings
   if(GetRootComponent()->Mobility.GetValue() == EComponentMobility::Movable)
      SetActorTransform(interactableInfo.transform); 

   // Reloading the level should save the decorators, but we have to restore state of the trigger decorators
   UInteractableActorDecoratorBase* decor = decorator;


   // Loop through the decorators set on this actor during level editing to find trigger decorators
   while (decor != nullptr) {
      if (decor->GetClass() == UTriggerInteractableDecorator::StaticClass()) {
         // Restore state of all trigger decorators
         UTriggerInteractableDecorator* triggerDecor = Cast<UTriggerInteractableDecorator>(decor);
         // Ensure we have the same number of triggers saved as the data we loaded
         check(triggerDecor->triggersActivatedOnInteract.Num() == interactableInfo.triggerDecoratorInfo.Num());
         for (int i = 0; i < triggerDecor->triggersActivatedOnInteract.Num(); ++i) {
            triggerDecor->triggersActivatedOnInteract[i].numCalls = interactableInfo.triggerDecoratorInfo[i].numCalls;
            triggerDecor->triggersActivatedOnInteract[i].enabled  = interactableInfo.triggerDecoratorInfo[i].enabled;
         }
         break;
      }
      decor = decor->decoratedInteractable;
   }

   // If for some reason we don't have a trigger decorator but the load data says we need one (sometimes this gets deleted during hotreload)
   if (!decor && interactableInfo.triggerDecoratorInfo.Num() > 0) {
      UE_LOG(LogActor, Error, TEXT("Warning, interactable base missing trigger decorator.  Creating placeholder but it needs to be populated with right data"));
      decor = NewObject<UTriggerInteractableDecorator>(this);
   }
}

void AInteractableBase::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);
}

FText AInteractableBase::GetGameName() const
{
   if (decorator) return decorator->GetName();
   return FText::GetEmpty();
}

void AInteractableBase::SetGameName(FText value)
{
   UInteractableActorDecoratorBase* nextDecorator = decorator;
   while (nextDecorator)
      ;
   {
      nextDecorator = nextDecorator->decoratedInteractable;
   }
   nextDecorator->decoratedInteractable = NewObject<UNamedInteractableDecorator>(nextDecorator);
}

void AInteractableBase::Interact_Implementation(ABaseHero* hero)
{
   if (IInteractable::Execute_CanInteract(this)) // calls Interact on decorator in the process
   {
      GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UGameplayDelegateContext>()->OnInteracted().Broadcast(GetClass(), GetGameName());
   }
}

FVector AInteractableBase::GetInteractableLocation_Implementation() const
{
   return sceneLocation->GetComponentLocation();
}

bool AInteractableBase::CanInteract_Implementation() const
{
   if (!decorator || decorator->Interact()) return true;
   return false;
}

void AInteractableBase::SaveInteractable(FMapSaveInfo& mapData)
{
   mapData.interactablesInfo.Add(SaveInteractableData());
}

void AInteractableBase::LoadInteractable(FMapSaveInfo& mapData)
{
   FInteractableSaveInfoWrapper* interactableInfo = mapData.interactablesInfo.FindByHash<AInteractableBase*>(GetTypeHash(*this), this);
   if(interactableInfo)
      LoadInteractableData(interactableInfo->interactableInfo);
}
