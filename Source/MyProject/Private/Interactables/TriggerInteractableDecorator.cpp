// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "TriggerInteractableDecorator.h"
#include "RTSTrigger.h"
#include "RTSGameMode.h"

UTriggerInteractableDecorator::UTriggerInteractableDecorator()
{
}

void UTriggerInteractableDecorator::Init()
{
   UInteractableActorDecoratorBase::Init();
}

bool UTriggerInteractableDecorator::Interact()
{
   if(!decoratedInteractable || decoratedInteractable->Interact())
   {
      for(URTSTriggerBase* Trigger : triggersActivatedOnInteract)
      {
         if(Trigger)
         {
            Trigger->TriggerEvent();
         }
      }
      return true;
   }
   return false;
}
