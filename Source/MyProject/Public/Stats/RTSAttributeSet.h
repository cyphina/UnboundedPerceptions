#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RTSAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)          \
   GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
   GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
   GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
   GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// We can use this to see the value of the macro (we need two layers else STR will just stringify the parameter instead of running the macro first)
//#define XSTR(x) STR(x)
//#define STR(x) #x
//
//   const FName x = XSTR(DECLARE_RTS_STAT_PROPERTY(Strength));

class AUnit;

DECLARE_EVENT_ThreeParams(URTSAttributeSet, FOnStatsUpdated, const FGameplayAttribute&, float, AUnit*);

/**
* Attribute set corresponding to the information held within our units
*/
UCLASS()
<<<<<<< HEAD:Source/MyProject/Public/Stats/MyAttributeSet.h
class MYPROJECT_API UMyAttributeSet : public UAttributeSet
=======
class MYPROJECT_API URTSAttributeSet : public UAttributeSet
>>>>>>> componentrefactor:Source/MyProject/Public/Stats/RTSAttributeSet.h
{
   GENERATED_BODY()

   static TSet<FGameplayAttribute> attSet;
   class AUserInput*               cpcRef;

 public:
<<<<<<< HEAD:Source/MyProject/Public/Stats/MyAttributeSet.h
   UMyAttributeSet();
   static const float MAX_HEALTH;
=======
   URTSAttributeSet();
>>>>>>> componentrefactor:Source/MyProject/Public/Stats/RTSAttributeSet.h

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
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Cosmic_Aff;

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
   UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData Cosmic_Resist;
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
   UPROPERTY(Category = "Mechanics", EditAnywhere, BlueprintReadWrite)
   FGameplayAttributeData HealthRegenModifier;

   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Strength);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Intelligence);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Agility);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Understanding);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Endurance);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Luck);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Explosiveness);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Critical_Chance);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Critical_Damage);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Accuracy);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Dodge);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Attack_Speed);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Cast_Speed);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Physical_Aff);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Fire_Aff);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Water_Aff);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Wind_Aff);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Earth_Aff);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Electric_Aff);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Darkness_Aff);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Light_Aff);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Arcane_Aff);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Chaos_Aff);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Poison_Aff);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Blood_Aff);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Ethereal_Aff);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Cosmic_Aff);

   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Physical_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Fire_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Water_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Wind_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Earth_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Electric_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Darkness_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Light_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Arcane_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Chaos_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Poison_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Blood_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Ethereal_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Cosmic_Resist);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Health);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Mana);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Psyche);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Moxie);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, Shield);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, MovementSpeed);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, AttackRange);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, WeaponPower);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, GlobalDamageModifier);
   ATTRIBUTE_ACCESSORS(URTSAttributeSet, HealthRegenModifier);

   static const TArray<FGameplayAttribute>& GetAtts();
   TArray<FGameplayAttribute>               GetSkills();
   TArray<FGameplayAttribute>               GetVitals();
   TArray<FGameplayAttribute>               GetMechanics();

   void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& data) override;

   // Helper so we can upgrade our stats.  Just a temporary fix, we'll find a better way later
   static FGameplayAttribute IndexAtts(int index);

   /**
    * @brief Called before one of the base attributes changes. It is used to recalculate other stats after an attribute change, but you can hook other things onto it.
    * @param Attribute 
    * @param NewValue 
    */
   void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override final;
<<<<<<< HEAD:Source/MyProject/Public/Stats/MyAttributeSet.h

   /**
    * @brief Called before any stat changes. Can be used to clamp attributes, or even to have event driven UI modifications.
    * @param Attribute 
    * @param NewValue 
    */
   void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override final;

   FOnStatsUpdated statUpdatedEvent;
   FOnStatsUpdated baseStatUpdatedEvent;
=======

   /**
    * @brief Called before any stat changes. Can be used to clamp attributes, or even to have event driven UI modifications.
    * @param Attribute 
    * @param NewValue 
    */
   void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override final;

   mutable FOnStatsUpdated statUpdatedEvent;
   mutable FOnStatsUpdated baseStatUpdatedEvent;
>>>>>>> componentrefactor:Source/MyProject/Public/Stats/RTSAttributeSet.h
};
