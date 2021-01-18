#include "MyProject.h"
#include "RTSConditional.h"
#include "NamedInteractableDecorator.h"
#include "GameplayDelegateContext.h"

bool UNamedInteractableDecorator::Interact()
{
   if (!decoratedInteractable || decoratedInteractable->Interact()) {
      GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UGameplayDelegateContext>()->OnInteracted().Broadcast(name);
      return true;
   }
   return false;
}
