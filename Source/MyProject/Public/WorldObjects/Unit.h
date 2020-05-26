#pragma once

#include "AIController.h" //Need this to add to delegate OnMoveCompleted

#include "CombatParameters.h"
#include "UnitProperties.h"

#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "UnitTargetData.h"

#include "GameplayTags.h"

#include "SaveLoadClass.h"
#include "UpDamageComponent.h"
#include "SpellSystem/UnitSpellData.h"
#include "State/IUnitState.h"

#include "State/StateMachine.h"
#include "Stats/BaseCharacter.h"

#include "WorldObject.h"
#include <typeinfo>
#include "Unit.generated.h"

class UDamageEffect;

DECLARE_DELEGATE(OnAttackAnimationTrigger); // Called when the attack animation finally

DECLARE_STATS_GROUP(TEXT("RTSUnits"), STATGROUP_RTSUnits, STATCAT_Advanced);

/** Used so we can store enemy unit data in a table*/
USTRUCT(BlueprintType)
struct FUnitInfoRow : public FTableRowBase {
   GENERATED_BODY()

 public:
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FText name;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   UTexture2D* image;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int strength;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int understanding;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int intelligence;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int explosiveness;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int endurance;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int agility;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int luck;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int baseResist;

   /**Ids 19-31*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   TMap<int, int> additionalResist;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int baseAffinity;

   /**Ids 6-18*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   TMap<int, int> additionalAffinities;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int baseHealth;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int mana;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int movespeed;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int attackrange;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int globalDamageModifier;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   TSubclassOf<AUnit> unitClass;
};

/** Base class for all things in the world that fights!*/
UCLASS(Abstract, HideCategories = (Movement, Optimization, Character, Camera, Pawn, CharacterMovement, Lighting, Rendering, Input, Actor))
class MYPROJECT_API AUnit : public ACharacter, public IWorldObject, public IAbilitySystemInterface
{
   GENERATED_BODY()

   static const int CONFIRM_SPELL_ID        = 1003;
   static const int CONFIRM_SPELL_TARGET_ID = 1004;

   friend class AUnitController;

 public:
   AUnit(const FObjectInitializer& oI);

   /** Messages we can send to the controller to let it know something happened*/
#pragma region listeningMessages
   static const FName AIMessage_AttackReady;
   static const FName AIMessage_SpellCasted;
   static const FName AIMessage_SpellInterrupt;
   static const FName AIMessage_Stunned;
   static const FName AIMessage_Silenced;
   static const FName AIMessage_TargetLoss;
   static const FName AIMessage_FoundTarget;
#pragma endregion

   /** Information about this unit and important references/components*/
#pragma region UnitInfoAndReferences

 protected:
   /** Struct holding some editable unit properties */
   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldObject Classification", meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
   FUnitProperties unitProperties;

   class AUserInput* controllerRef;

   TUniquePtr<StateMachine>   state = nullptr; // Reference to statemachine
   TUniquePtr<FBaseCharacter> baseC = nullptr; // Reference to statmanager.  Is a pointer so we can make it and give it a reference to the attribute set

   UPROPERTY()
   class AUnitController* unitController = nullptr; // Reference to AIController

   UPROPERTY()
   class URTSUnitAnim* animRef = nullptr;

#pragma endregion

   /** Attached components*/
#pragma region components
 public:
   /** Since we perform vision calculations on another thread, we need this mutex to prevent data races */
   FWindowsRWLock visionMutex;

   /** Component that hangs over our head to visually denote how much health we have left*/
   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   class UHealthbarComp* healthBar;

   /** Material reference to unit's base material*/
   UPROPERTY(EditAnywhere)
   UMaterialInterface* material;

   /** Selection circle decal below unit*/
   UPROPERTY(VisibleAnywhere)
   UDecalComponent* selectionCircleDecal;

   /** Sphere representing vision radius
    * Issue https://community.gamedev.tv/t/syntax-error-missing-before/66365
    * and https://stackoverflow.com/questions/28007165/class-keyword-in-variable-definition-in-c/28007230 (inline forward declaration)
    * depicting why we use class
    */
   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   class USphereComponent* visionSphere;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   UUpDamageComponent* damageComponent;

#pragma endregion

   /** Functions in reponse to events */
#pragma region Callbacks

   void BeginPlay() override;
   void Tick(float deltaSeconds) override;
   void PossessedBy(AController* newController) override;

   /**Change speed based parameters when time multiplier changes*/
   UFUNCTION(Category = "Callback")
   void OnUpdateGameSpeed(float speedMultiplier);

   /**Function called when unit dies :(*/
   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functionality")
   void         Die();
   virtual void Die_Implementation();

 private:
   /** Callback function for move completed. Delegates can work with virtual functions. */
   UFUNCTION()
   void OnMoveCompleted(FAIRequestID RequestID, const EPathFollowingResult::Type Result);

#pragma endregion

   /** Private data member interface */
#pragma region Accessors
 public:
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   EUnitState GetState() const; // Gets name of current state in statemachine

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FORCEINLINE AUnitController* GetUnitController() const { return unitController; }

   UFUNCTION(BlueprintCallable, Category = "Accessors")
   void SetGameName(FText value) final override { unitProperties.name = value; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FText GetGameName() const final override { return unitProperties.name; }

   UFUNCTION(BlueprintCallable, Category = "Accessors")
   void SetImage(UTexture2D* value) { unitProperties.image = value; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   UTexture2D* GetImage() const { return unitProperties.image; }

   UFUNCTION(BlueprintCallable, Category = "Accessors")
   virtual void SetSelected(bool value) { unitProperties.isSelected = value; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   virtual bool GetSelected() const { return unitProperties.isSelected; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   bool GetIsEnemy() const { return combatParams.isEnemy; }

   /** Call this function over setting values manually (which will be made private one day) because it sets the targetActor and targetUnit, both which are used
    * when dealing with unit tasks. */
   UFUNCTION(BlueprintCallable, Category = "CombatAccessors")
   void SetTarget(AUnit* value)
   {
      targetData.targetUnit  = value;
      targetData.targetActor = value;
   }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   AUnit* GetTarget() const { return targetData.targetUnit; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   FVector GetTargetLoc() const { return targetData.targetLocation; }

   UFUNCTION(BlueprintCallable, Category = "CombatAccessors")
   void SetCanTarget(bool value) { unitProperties.canTarget = value; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   bool GetCanTarget() const { return unitProperties.canTarget; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   bool GetIsDead() const { return combatParams.isDead; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   bool GetIsReadyToAttack() const { return combatParams.readyToAttack; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   int GetVisionRadius() const { return unitProperties.visionRadius; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   float GetDPS(float timespan);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   float GetDamageRecievedPerSecond(float timespan);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   float GetHealingPerSecond(float timespan);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   float GetHealingRecievedPerSecond(float timespan);

   // float GetStunPerSecond(float timespan);
   const FBaseCharacter& GetBaseCharacter() const { return *baseC; }

   /** Expose stats to blueprints and other units but only change the stats from C++ which is why we don't see any setters */
#pragma region Stats

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE int GetAttributeBaseValue(EAttributes att) const { return baseC->GetAttribute(static_cast<uint8>(att))->GetBaseValue(); };

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetSkillBaseValue(EUnitScalingStats skill) const { return baseC->GetSkill(static_cast<uint8>(skill))->GetBaseValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetVitalBaseValue(EVitals vit) const { return baseC->GetVital(static_cast<uint8>(vit))->GetBaseValue(); };

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetMechanicBaseValue(EMechanics mec) const { return baseC->GetMechanic(static_cast<uint8>(mec))->GetBaseValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE int GetAttributeAdjValue(EAttributes att) const { return baseC->GetAttribute(static_cast<uint8>(att))->GetCurrentValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetSkillAdjValue(EUnitScalingStats skill) const { return baseC->GetSkill(static_cast<uint8>(skill))->GetCurrentValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetMechanicAdjValue(EMechanics mec) const { return baseC->GetMechanic(static_cast<uint8>(mec))->GetCurrentValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetVitalAdjValue(EVitals vit) const { return baseC->GetVital(static_cast<uint8>(vit))->GetCurrentValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetVitalCurValue(EVitals vit) const { return baseC->GetVital(static_cast<uint8>(vit))->GetCurrentValue(); }

   /**Only used to force hp to change to another value by triggers or cheatmenu*/
   UFUNCTION(BlueprintCallable, Category = "StatAccessors")
   void SetVitalCurValue(int vit, int vitValue) const { baseC->SetVitalAdj(vit, vitValue); }

   /**Get Level of unit from baseCharacter*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE int GetLevel() const { return baseC->GetLevel(); }

   UFUNCTION(BlueprintCallable, Category = "Stats")
   void UpdateStats(const FGameplayAttribute& updatedStat); // call whenever stats get changed

   /**Allows us to apply some bonuses to this hero's stat
    * @template  StatType - Enum representing what stat to modify
    * @param value - New value for the stat
    * @param specificStatType - Actual enum value
    * @param bModifyBase - Modify the base stat or the adjusted value
    */
   template <bool bModifyBase = false, typename StatType>
   void ModifyStats(int value, StatType specificStatType)
   {
      static_assert(std::is_enum<StatType>::value, "Must pass one of the stat Enums!");
      const int specificStatTypeVal = static_cast<int>(specificStatType);

      if constexpr(std::is_same<StatType, EAttributes>::value) {
         if constexpr(!bModifyBase) {
            baseC->SetAttributeAdj(specificStatTypeVal, value);
         } else {
            baseC->SetAttributeBase(specificStatTypeVal, value);
         }
      } else if constexpr(std::is_same<StatType, EUnitScalingStats>::value) {
         if constexpr(!bModifyBase) {
            baseC->SetSkillAdj(specificStatTypeVal, value);
         } else {
            baseC->SetSkillBase(specificStatTypeVal, value);
         }
      } else if constexpr(std::is_same<StatType, EVitals>::value) {
         if constexpr(!bModifyBase) {
            baseC->SetVitalAdj(specificStatTypeVal, value);
         } else {
            baseC->SetVitalBase(specificStatTypeVal, value);
         }
      } else if constexpr(std::is_same<StatType, EMechanics>::value) {
         if constexpr(!bModifyBase) {
            baseC->SetMechanicAdj(specificStatTypeVal, value);
         } else {
            baseC->SetMechanicBase(specificStatTypeVal, value);
         }
      } else {
         static_assert(std::is_same_v<StatType*, void>, "Wee");
      }
   }

   // Allows us to deduce the type of the enum and call the above function so we can call the above function using an enum template argument if we know it at compile time
   //template <auto EnumVal, bool bModifyBase = false>
   //void ModifyStats(int value)
   //{
   //   ModifyStats<bModifyBase, decltype(EnumVal)>(value, EnumVal);
   //}

#pragma endregion

#pragma endregion

 public:
   /** Function to find the bounds of a unit (screen space points) */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
   FBox2D FindBoundary() const;

   /** Function to disable (pretty much erases its trace from the world) but not destroy this actor in memory*/
   UFUNCTION(BlueprintCallable, Category = "Functionality")
   virtual void SetEnabled(bool bEnabled);

   /** Combat related functions and parameters */
#pragma region combat

   /** Get data on our spell target.  Is reset after the spell is sucessfully casted*/
   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   FGameplayAbilityTargetDataHandle GetTargetData() const { return targetData.spellTargetData; }

   /** Get data on targetted unit.  Used in single target spellcasting and attacking.
    * Reset after spell casted and if we attack a new target or stop.
    * Faster to use this then cast GetTargetData() to some unit type */
   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   AUnit* GetTargetUnit() const { return targetData.targetUnit; }

   /** Damage Calculations on Receiving End.  If using some magic armor, attacker may get some debuffs */
   friend void UUpDamageComponent::DamageTarget(FUpDamage& d, FGameplayTagContainer effects) const;
   friend void UUpDamageComponent::UnitDamageTarget(FUpDamage& d, FGameplayTagContainer effects) const;

   /** Play attack animation and generate any visual effects.  If no animation to handle Attack notifications, will call Attack() manually*/
   void PlayAttackEffects();

 protected:
   UnitTargetData       targetData;   // Holds data for all this unit's current possible targets
   UPAICombatParameters combatParams; // Holds some functionality and information about combat

   /** Process to create damage effect */
   UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
   void         Attack();
   virtual void Attack_Implementation();

 public:
   /** Stunned prevents any action from being taken by the unit and should disrupt any queued actions as well*/
   UFUNCTION(BlueprintCallable)
   bool IsStunned() const;

   /** Prevents spellcasting*/
   UFUNCTION(BlueprintCallable)
   bool IsSilenced() const;

   /**Check to see if the unit has any statuses that cause the unit to be considered invisible and not revealed by true sight*/
   UFUNCTION(BlueprintCallable)
   bool IsInvisible() const;

   /**Create some text to display the damage dealt*/
   void ShowDamageDealt(const FUpDamage& damage);

   inline UAbilitySystemComponent* GetAbilitySystemComponent() const override;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FORCEINLINE TSubclassOf<UMySpell> GetCurrentSpell() const { return unitSpellData.currentSpell; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   float GetChannelTime() const { return unitSpellData.channelTime; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   float GetCurrentChannelTime() const { return unitSpellData.currentChannelTime; }

   UPROPERTY(EditAnywhere, Category = "Animations")
   UAnimMontage* moveAnimation = nullptr;

   UPROPERTY(EditAnywhere, Category = "Animations")
   UAnimMontage* attackAnimation = nullptr;

   UPROPERTY(EditAnywhere, Category = "Animations")
   UAnimMontage* castAnimation = nullptr;

#pragma endregion

   /** Skill related functions and parameters */
#pragma region Skills
 public:
   /** List of abilities that are in unit's skill slots */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
   TArray<TSubclassOf<class UMySpell>> abilities;

   /** Gets CDO of any spell.  CDOs of objects can only be given a struct, this function lets us get
    * the CDO as the type of the object.  Also checks to see if hero has spell, else it won't work
    * Trying to get instances only returns active instances, that is, spells
    * that are marked "Active" and haven't been committed because they are on pause.
    * Use GetSpellInstance to get actual instance */
   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   UMySpell* GetSpellCDO(TSubclassOf<UMySpell> spellClass) const;

   /** Do we have the resources necessary to cast this spell */
   bool CanCast(TSubclassOf<UMySpell> spellToCheck);

 protected:
   /** Sets the spell currently being channeled */
   inline void SetCurrentSpell(TSubclassOf<UMySpell> newCurrentSpell) { unitSpellData.currentSpell = newCurrentSpell; }

   /** Actually activates the ability when we're in range or  and triggers abilities that
    * wait for click events to continue
    * @param spellToCast - Spell we want to cast.  Left as a parameter because we can cast
    * spells that aren't our current spells in some scenarios like item usage */
   virtual bool CastSpell(TSubclassOf<UMySpell> spellToCast);

   UnitSpellData unitSpellData;

#pragma endregion

   /** Parameters used to record what units and parts of the map are visible */
#pragma region vision
 public:
   int GetVisionCount() const { return enemyVisionCount; }

   void IncVisionCount() { ++enemyVisionCount; }

   void DecVisionCount() { --enemyVisionCount; }

   virtual bool IsVisible() PURE_VIRTUAL(AUnit::IsVisible, return false;);

   // Check used to see if unit is visible from the perspective of the team the unit is on
   virtual TSet<AUnit*>* GetSeenEnemies() PURE_VIRTUAL(AUnit::GetSeenEnemies, return nullptr;);

 private:
   /** Counter for number of enemies (units with opposite value of isEnemy) that can see this unit */
   int enemyVisionCount;

   friend void USaveLoadClass::SetupBaseCharacter(AAlly* spawnedAlly, FBaseCharacterSaveInfo& baseCSaveInfo);

   friend class IncantationState;
   friend class ChannelingState;
   friend class CastingState;
   friend class AttackState;
   friend class MovingState;
   friend class AttackMoveState;
   friend class ChasingState;

#pragma endregion

   /** Parameters and functions used to calculate certain values in the AI blackboard */
#pragma region AIValues

 private:
   static const float diminishParam;

 protected:
   static const TFunction<float(float)> diminishFunc;

 public:
   /** Parameter used in AI calculations to determine how much this unit is at risk of dying*/
   UFUNCTION(BlueprintCallable, BlueprintPure)
   float CalculateRisk();

   /** Parameter used in AI calculations to determine how threatening this unit is*/
   UFUNCTION(BlueprintCallable, BlueprintPure)
   float CalculateThreat();

   virtual float CalculateTargetRisk() PURE_VIRTUAL(AUnit::CalculateTargetRisk, return 0.f;);

#pragma endregion
};
