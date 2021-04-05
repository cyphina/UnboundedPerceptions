#include "MyProject.h"
#include "UpResourceManager.h"
#include "BaseCharacter.h"
#include "BasePlayer.h"

float UpResourceManager::FindOrientation(const FVector& v)
{
   const bool  positiveX     = v.X >= 0;
   FVector     up            = positiveX ? FVector::RightVector : -FVector::RightVector;
   const float normalizedDot = (up.X * v.X + up.Y * v.Y) / (FMath::Sqrt(v.SizeSquared()));
   return positiveX ? normalizedDot + 2 : normalizedDot;
}

void UpResourceManager::ExecuteFunctionFromWorldObject(UObject* objectRef, FName functionToExecute)
{
   if(objectRef)
   {
      UFunction* function = objectRef->FindFunction(functionToExecute);
      if(function)
      {
         // pointer to memory of local variables in stack
         void* locals = nullptr;
         // creates the stack frame for the function
         TUniquePtr<FFrame> frame = TUniquePtr<FFrame>(new FFrame{objectRef, function, locals});
         // call the UFunction
         // processEvent (if it has params)
         objectRef->CallFunction(*frame, locals, function);
      }
   }
}

template <>
ABaseHero* UpResourceManager::FindTriggerObjectInWorld<ABaseHero>(FStringView nameToMatch, UWorld* worldRef)
{
   AUserInput* cpcRef = Cast<AUserInput>(worldRef->GetFirstPlayerController());
   for(ABaseHero* hero : cpcRef->GetBasePlayer()->allHeroes)
   {
      if(hero->GetGameName().ToString() == nameToMatch)
      {
         return Cast<ABaseHero>(hero);
      }
   }
   return nullptr;
}
