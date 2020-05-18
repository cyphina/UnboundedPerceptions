#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MyAttributeSet.generated.h"

/**
 * Attribute set corresponding to the information held within our units
 */

#define DECLARE_ATTRIBUTE_FUNCTION(PropertyName) static FGameplayAttribute PropertyName##Attribute();
#define DEFINE_ATTRIBUTE_FUNCTION(PropertyName, ClassName)                                                                                     \
   FGameplayAttribute ClassName## ::PropertyName##Attribute()                                                                                  \
   {                                                                                                                                           \
      static UProperty* Property = FindFieldChecked<UProperty>(ClassName## ::StaticClass(), GET_MEMBER_NAME_CHECKED(ClassName, PropertyName)); \
      return FGameplayAttribute(Property);                                                                                                     \
   }

// Memory of me trying to create a macro to save code.  But the UHT can't parse it since it relies on the UPROPERTY() text being written, and it thinks the macros are unknown types
// #define DECLARE_RTS_STAT_PROPERTY(PropertyName, CategoryName) \
//    UPROPERTY(Category = #CategoryName, EditAnywhere, BlueprintReadWrite) \
//    FGameplayAttributeData PropertyName##;

// We can use this to see the value of the macro (we need two layers else STR will just stringify the parameter instead of running the macro first)
//#define XSTR(x) STR(x)
//#define STR(x) #x
//
//   const FName x = XSTR(DECLARE_RTS_STAT_PROPERTY(Strength));

class AUnit;

DECLARE_EVENT_ThreeParams(UMyAttributeSet, FOnStatsUpdated, const FGameplayAttribute&, float&, AUnit*);

UCLASS()
class MYPROJECT_API UMyAttributeSet : public UAttributeSet {
   GENERATED_BODY()

   static TArray<FGameplayAttribute> attList;
   static TSet<FGameplayAttribute>   attSet;

   UPROPERTY()
   class AUserInput* cpcRef;

public:

   UMyAttributeSet();
   static const float MAX_HEALTH;

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

   void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& data) override;

   DECLARE_ATTRIBUTE_FUNCTION(Strength);
   DECLARE_ATTRIBUTE_FUNCTION(Intelligence);
   DECLARE_ATTRIBUTE_FUNCTION(Agility);
   DECLARE_ATTRIBUTE_FUNCTION(Understanding);
   DECLARE_ATTRIBUTE_FUNCTION(Endurance);
   DECLARE_ATTRIBUTE_FUNCTION(Luck);
   DECLARE_ATTRIBUTE_FUNCTION(Explosiveness);
   DECLARE_ATTRIBUTE_FUNCTION(Critical_Chance);
   DECLARE_ATTRIBUTE_FUNCTION(Critical_Damage);
   DECLARE_ATTRIBUTE_FUNCTION(Accuracy);
   DECLARE_ATTRIBUTE_FUNCTION(Dodge);
   DECLARE_ATTRIBUTE_FUNCTION(Attack_Speed);
   DECLARE_ATTRIBUTE_FUNCTION(Cast_Speed);
   DECLARE_ATTRIBUTE_FUNCTION(Physical_Aff);
   DECLARE_ATTRIBUTE_FUNCTION(Fire_Aff);
   DECLARE_ATTRIBUTE_FUNCTION(Water_Aff);
   DECLARE_ATTRIBUTE_FUNCTION(Wind_Aff);
   DECLARE_ATTRIBUTE_FUNCTION(Earth_Aff);
   DECLARE_ATTRIBUTE_FUNCTION(Electric_Aff);
   DECLARE_ATTRIBUTE_FUNCTION(Darkness_Aff);
   DECLARE_ATTRIBUTE_FUNCTION(Light_Aff);
   DECLARE_ATTRIBUTE_FUNCTION(Arcane_Aff);
   DECLARE_ATTRIBUTE_FUNCTION(Chaos_Aff);
   DECLARE_ATTRIBUTE_FUNCTION(Poison_Aff);
   DECLARE_ATTRIBUTE_FUNCTION(Blood_Aff);
   DECLARE_ATTRIBUTE_FUNCTION(Ethereal_Aff);
   DECLARE_ATTRIBUTE_FUNCTION(Physical_Resist);
   DECLARE_ATTRIBUTE_FUNCTION(Fire_Resist);
   DECLARE_ATTRIBUTE_FUNCTION(Water_Resist);
   DECLARE_ATTRIBUTE_FUNCTION(Wind_Resist);
   DECLARE_ATTRIBUTE_FUNCTION(Earth_Resist);
   DECLARE_ATTRIBUTE_FUNCTION(Electric_Resist);
   DECLARE_ATTRIBUTE_FUNCTION(Darkness_Resist);
   DECLARE_ATTRIBUTE_FUNCTION(Light_Resist);
   DECLARE_ATTRIBUTE_FUNCTION(Arcane_Resist);
   DECLARE_ATTRIBUTE_FUNCTION(Chaos_Resist);
   DECLARE_ATTRIBUTE_FUNCTION(Poison_Resist);
   DECLARE_ATTRIBUTE_FUNCTION(Blood_Resist);
   DECLARE_ATTRIBUTE_FUNCTION(Ethereal_Resist);
   DECLARE_ATTRIBUTE_FUNCTION(Health);
   DECLARE_ATTRIBUTE_FUNCTION(Mana);
   DECLARE_ATTRIBUTE_FUNCTION(Psyche);
   DECLARE_ATTRIBUTE_FUNCTION(Moxie);
   DECLARE_ATTRIBUTE_FUNCTION(Shield);
   DECLARE_ATTRIBUTE_FUNCTION(MovementSpeed);
   DECLARE_ATTRIBUTE_FUNCTION(AttackRange);
   DECLARE_ATTRIBUTE_FUNCTION(WeaponPower);
   DECLARE_ATTRIBUTE_FUNCTION(GlobalDamageModifier);

   TArray<FGameplayAttribute> GetAtts();
   TArray<FGameplayAttribute> GetSkills();
   TArray<FGameplayAttribute> GetVitals();
   TArray<FGameplayAttribute> GetMechanics();

   // Helper so we can upgrade our stats.  Just a temporary fix, we'll find a better way later
   static FGameplayAttribute IndexAtts(int index);

   // PreAttributeBaseChange -- called before base value changed
   void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override final;
   // Called before an attribute changes allowing us to update our stat UI
   void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override final;

   FOnStatsUpdated statUpdatedEvent;
   FOnStatsUpdated baseStatUpdatedEvent;

};
