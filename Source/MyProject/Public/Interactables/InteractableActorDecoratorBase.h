#pragma once
#include "BaseHero.h"
#include "InteractableActorDecoratorBase.generated.h"

/** Decorator actor base (since we can't have pointers to interfaces)
 * Decorators are almost like a linked list...  The only benefit they have over storing an array of objects is that we can keep the idea of looping over the
 * objects to add functionality transparent.

 * Initially I wanted to make interactable decorators (decorator pattern).  There would be a trigger decorator, conditional decorator, name decorator, etc.
 * However the decorator pattern is probably impossible to implement properly with unreal limitations with the constructors and my desire to keep properties exposed
 * to the editor.
 *
 * However we can make a interactable spawner (not the spawner interactable) that spawns interactable.  The information about the interactable would be
 * in the spawner, and the interactable would just hold information about itself (e.g., the door would hold its rotation information while the spawner would hold
 * the condition, trigger, etc.)
 *
 * Eventually I found out how to make a scuffed decorator pattern.  This isn't exactly like the pattern, but it works since we can't use AActor's when exposing properties
 * via Instanced, so I had to adapt my plan so I can get the main benefit of being able to edit it in the property window.
 */

/**Decorators add extra functionality to the interactables, like activating triggers, forcing conditionals for interaction, and adding additional information to them.
 *To use, choose a decorator in the interactable's decorator field.  In the decorator itself, there's a decoratedInteractable field that can be filled out to
 *chain decorators together.  Don't add more of the same type since it's not necessary.
 */

UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class MYPROJECT_API UInteractableActorDecoratorBase : public UObject
{
   GENERATED_BODY()

 public:
   /**The next decorator to add to this interactable (if there is one).  Kind of like a node in a linked list, but each decorator adds some functionality*/
   UPROPERTY(EditAnywhere)
   UInteractableActorDecoratorBase* decoratedInteractable;

   /**Initialize some values for the decorator that need to be initialized usually at BeginPlay.  Called by the InteractableBase to setup the decorator during
    *InteractableBase::BeginPlay()
    */
   virtual void Init()
   {
      if (decoratedInteractable) {
         decoratedInteractable->Rename(0, this);
         decoratedInteractable->Init();
      }
   }

   /**Performs extra work during Interact() call in the InteractableBase as well as has some usage as a boolean to make sure any conditionalDecorators are sucessfully
    *activated (if there are any in the decorator chain*/
   virtual bool Interact() PURE_VIRTUAL(UInteractableActor::Interact, return true;);

   virtual FText GetName() { return decoratedInteractable->GetName(); }
};