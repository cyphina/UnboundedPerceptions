#include "MyProject.h"
#include "NamedInteractableDecorator.h"

bool UNamedInteractableDecorator::Interact()
{
   if (!decoratedInteractable || decoratedInteractable->Interact()) {
      return true;
   }
   return false;
}
