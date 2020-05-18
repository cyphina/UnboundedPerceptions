// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "UserInput.h"
#include "BasePlayer.h"
#include "WorldObjects/BaseHero.h"

#if UE_EDITOR
#include <type_traits>
#endif

/** Singleton class for some important variables */
namespace UpResourceManager
{
   class FGameplayAttributeData;
   class ABaseCharacter;

   // maps a gameplay tag representing an element to its corresponding color
   extern TMap<FGameplayTag, FColor> elementalMap;

   // Used to get bounds of some object with collision (represents the corners of a cube)
   const FVector BoundsPointMapping[8] = { FVector(1.f, 1.f, 1.f), FVector(1.f, 1.f, -1.f), FVector(1.f, -1.f, 1.f), FVector(1.f, -1.f, -1.f),
                                          FVector(-1.f, 1.f, 1.f), FVector(-1.f, 1.f, -1.f), FVector(-1.f, -1.f, 1.f), FVector(-1.f, -1.f, -1.f) };

   const FVector BoundsPointMapping2D[4] = {
   FVector(1.f, 1.f, 0.f),
   FVector(1.f, -1.f, 0.f),
   FVector(-1.f, -1.f, 0.f),
   FVector(-1.f, 1.f, -1.f),
   };

   struct StatKeyFunc : TDefaultMapHashableKeyFuncs<uint8, uint8, false> {

      /** Maps 0-NumAtts to 0, NumAtts-NumScalingStats to 1, and so forth */
      static FORCEINLINE uint32 GetKeyHash(uint8 Key)
      {
         checkf((unsigned)Key < (unsigned)(CombatInfo::TotalStatCount), TEXT("Don't map any values out of the number of stats %d"), Key);
         if (Key < CombatInfo::AttCount) {
            return 0;
         }
         if (Key < CombatInfo::AttCount + CombatInfo::StatCount) {
            return 1;
         }
         if (Key < CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::MechanicCount) {
            return 2;
         }
         return 3;
      }
   };

   // Maps stat num ( 0- Total Num Stats ) to enum ( 0 - Attributes, 1 - Stats, 2 - Vitals, 3 - Mechanics)
   const TMap<uint8, uint8, FDefaultSetAllocator, StatKeyFunc> statMapper = { {0, 0}, {CombatInfo::AttCount, 1},
      {CombatInfo::AttCount + CombatInfo::StatCount, 2},
      {CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount, 3} };

   /**Returns the Simpson quadrature's coefficient*/
   static const TFunction<float(float)> SimpsonSpacing = [](float space) { return 1.f / 3 * space; };
   /**Function used to get diminishing return values 1/(x+1)^n.*/
   const TFunction<float(float, float)> DiminishFunc = [](float x, float n) { return (FMath::Pow(x + 1.f, 1.f - n) - 1.f) / (1.f - n); };
   /**Simple numerical integration from 0 to x, primarily used to estimate integral of the diminishing function*/
   const TFunction<float(float, TFunction<float(float)>)> SimpsonApprox = [](float x, TFunction<float(float)> f)
   {
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Function values: %f, %f, %f"), f(0), f(x / 2), x));
      return SimpsonSpacing(x / 2) * (f(0) + 4 * f(x / 2) + f(x));
   };

   void InitUpResourceManager();
   void InitElementalMap();
   void InitSupportTags();
   void InitOffensiveTags();

   /**Helper to score how large a vector's angle is compared to the vector (0,1,0)*/
   float FindOrientation(const FVector& v);

   ///< summary>
   /// These template can generate alot of code if you use too many different types.  Probably not a problem considering how large codebase already is but just a note
   ///</summary>

   /**Make sure when you pass a world reference to this class, you pass it one from an actor or from a function that has (meta=WorldContextObject)
    *@param nameToMatch - Name of an object that implements IWorldObject
    *@param worldRef - Reference to the world (from an actor or uobject with an actor outer)
    */
   template <class T>
   T* FindTriggerObjectInWorld(FString nameToMatch, UWorld* worldRef)
   {
#if UE_EDITOR
      static_assert(std::is_base_of<IWorldObject, T>::value, "Template parameter should derive from IWorldObject");
#endif

      for (TActorIterator<T> actItr(worldRef); actItr; ++actItr) { if ((*actItr)->GetGameName().ToString() == nameToMatch) { return *actItr; } }

#if UE_EDITOR
      UE_LOG(LogTemp, Warning, TEXT("Could not find object named %s"), *nameToMatch);
#endif
      return nullptr;
   }

   template <>
   ABaseHero* FindTriggerObjectInWorld<ABaseHero>(FString nameToMatch, UWorld* worldRef);

   /**Executes a function inside an arbitrary UObject using UObject Reflection (granted T is a subclass of UObject)*/
   void ExecuteFunctionFromWorldObject(UObject* objectRef, FName functionToExecute);

   template <typename T>
   struct GetPropertyFromType;

   template <>
   struct GetPropertyFromType<float> {
      using value = UFloatProperty;
   };

   template <>
   struct GetPropertyFromType<int> {
      using value = UIntProperty;
   };

   /**Gets a property from a UObject given that the property is a UObject itself*/
   template <typename T>
   bool GetObjectVariable(UObject* objectRef, FName propertyToRead, T& outValue, UWorld* worldRef)
   {
      if (objectRef) {
         using Property = GetPropertyFromType<T>::value;
         Property p = FindField<Property>(wo->GetClass(), propertyToRead);
         if (p) {
            outValue = p->GetPropertyValue_InContainer(objectRef);
            return true;
         }
      }
      return false;
   }

   template <typename T>
   bool SetObjectVariable(UObject* objectRef, FName propertyToRead, T newValue, UWorld* worldRef)
   {
      if (objectRef) {
         using Property = GetPropertyFromType<T>::value;
         Property p = FindField<Property>(wo->GetClass(), propertyToRead);
         if (p) {
            outValue = p->SetPropertyValue_InContainer(objectRef, newValue);
            return true;
         }
      }
      return false;
   }

   // Tags that make an ability considered a support ability
   extern FGameplayTagContainer supportTags;
   extern FGameplayTagContainer offensiveTags;
} // namespace UpResourceManager
