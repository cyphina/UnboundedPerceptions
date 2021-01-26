#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MyAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// We can use this to see the value of the macro (we need two layers else STR will just stringify the parameter instead of running the macro first)
//#define XSTR(x) STR(x)
//#define STR(x) #x
//
//   const FName x = XSTR(DECLARE_RTS_STAT_PROPERTY(Strength));

class AUnit;

DECLARE_EVENT_ThreeParams(UMyAttributeSet, FOnStatsUpdated, const FGameplayAttribute&, float, AUnit*);

/**
* Attribute set corresponding to the information held within our units
*/
UCLASS()
class MYPROJECT_API UMyAttributeSet : public UAttributeSet
{
   GENERATED_BODY()

   static TArray<FGameplayAttribute> attList;
   static TSet<FGameplayAttribute>   attSet;

   UPROPERTY()
   class AUserInput* cpcRef;

 public:
   UMyAttributeSet();

   ///--Attributes--
   UPROPERTY(Category = "Attributes", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Strength;
   UPROPERTY(Category = "Attributes", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Understanding;
   UPROPERTY(Category = "Attributes", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Intelligence;
   UPROPERTY(Category = "Attributes", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Explosiveness;
   UPROPERTY(Category = "Attributes", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Endurance;
   UPROPERTY(Category = "Attributes", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Agility;
   UPROPERTY(Category = "Attributes", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Luck;

   ///--Stats--
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Critical_Chance;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Critical_Damage;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Accuracy;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Dodge;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Attack_Speed;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Cast_Speed;

   // Affinity
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Physical_Aff;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Fire_Aff;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Water_Aff;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Wind_Aff;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Earth_Aff;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Electric_Aff;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Darkness_Aff;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Light_Aff;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Arcane_Aff;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Chaos_Aff;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Poison_Aff;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Blood_Aff;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Ethereal_Aff;

   // Resistances
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Physical_Resist;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Fire_Resist;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Water_Resist;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Wind_Resist;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Earth_Resist;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Electric_Resist;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Darkness_Resist;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Light_Resist;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Arcane_Resist;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Chaos_Resist;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Poison_Resist;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Blood_Resist;
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Ethereal_Resist;

   ///--Vitals--
   UPROPERTY(Category = "Vitals", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Health;
   UPROPERTY(Category = "Vitals", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Mana;
   UPROPERTY(Category = "Vitals", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Psyche;
   UPROPERTY(Category = "Vitals", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Moxie;
   UPROPERTY(Category = "Vitals", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Shield;

   //--Mechanics--

   UPROPERTY(Category = "Mechanics", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData MovementSpeed;
   UPROPERTY(Category = "Mechanics", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData AttackRange;
   UPROPERTY(Category = "Mechanics", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData WeaponPower;
   UPROPERTY(Category = "Mechanics", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData GlobalDamageModifier;

   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Strength);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Intelligence);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Agility);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Understanding);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Endurance);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Luck);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Explosiveness);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Critical_Chance);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Critical_Damage);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Accuracy);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Dodge);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Attack_Speed);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Cast_Speed);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Physical_Aff);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Fire_Aff);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet,Water_Aff);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Wind_Aff);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Earth_Aff);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Electric_Aff);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Darkness_Aff);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Light_Aff);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Arcane_Aff);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Chaos_Aff);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Poison_Aff);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Blood_Aff);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Ethereal_Aff);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Physical_Resist);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Fire_Resist);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Water_Resist);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Wind_Resist);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Earth_Resist);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Electric_Resist);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Darkness_Resist);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Light_Resist);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Arcane_Resist);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Chaos_Resist);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Poison_Resist);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Blood_Resist);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Ethereal_Resist);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Health);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Mana);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Psyche);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Moxie);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, Shield);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, MovementSpeed);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, AttackRange);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, WeaponPower);
   ATTRIBUTE_ACCESSORS(UMyAttributeSet, GlobalDamageModifier);

   static const TArray<FGameplayAttribute>& GetAtts();
   TArray<FGameplayAttribute> GetSkills();
   TArray<FGameplayAttribute> GetVitals();
   TArray<FGameplayAttribute> GetMechanics();

   void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& data) override;

   // Helper so we can upgrade our stats.  Just a temporary fix, we'll find a better way later
   static FGameplayAttribute IndexAtts(int index);

   /**
    * @brief Called before one of the base attributes changes. It is used to recalculate other stats after an attribute change, but you can hook other things onto it.
    * @param Attribute 
    * @param NewValue 
    */
   void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override final;

   /**
    * @brief Called before any stat changes. Can be used to clamp attributes, or even to have event driven UI modifications.
    * @param Attribute 
    * @param NewValue 
    */
   void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override final;

   mutable FOnStatsUpdated statUpdatedEvent;
   mutable FOnStatsUpdated baseStatUpdatedEvent;
};
