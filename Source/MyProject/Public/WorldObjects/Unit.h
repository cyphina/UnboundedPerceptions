#pragma once

#include "AIController.h" //Need this to add to delegate OnMoveCompleted
#include "AbilitySystemInterface.h"
#include "CombatParameters.h"
#include "UnitProperties.h"
#include "GameFramework/Character.h"
#include "GameplayEffect.h"
#include "GameplayTags.h"
#include "SaveLoadClass.h"
#include "SpellSystem/Calcs/DamageCalculation.h"
#include "SpellSystem/UnitSpellData.h"
#include "State/IUnitState.h"
#include "State/StateMachine.h"
#include "Stats/BaseCharacter.h"
#include "UnitTargetData.h"
#include "WorldObject.h"
#include "Unit.generated.h"

class AUserInput;
class ARTSGameState;
class AUnitController;
class UDamageEffect;
class UHealthbarComp;

DECLARE_STATS_GROUP(TEXT("RTSUnits"), STATGROUP_RTSUnits, STATCAT_Advanced);

UCLASS(Abstract)
class MYPROJECT_API AUnit : public ACharacter, public IWorldObject, public IAbilitySystemInterface
{
   GENERATED_BODY()

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
#pragma endregion

/** Information about this unit and important references/components*/
#pragma region UnitInfoAndReferences

 protected:

   /** Struct holding some editable unit properties */
   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldObject Classification", meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
   FUnitProperties unitProperties;

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
   AUserInput* controllerRef = nullptr;

   TUniquePtr<StateMachine>   state          = nullptr; // Reference to statemachine
   TUniquePtr<FBaseCharacter> baseC          = nullptr; // Reference to statmanager.  Is a pointer so we can make it and give it a reference to the attribute set
   AUnitController*           unitController = nullptr; // Reference to AIController
   ARTSGameState*             gameState      = nullptr; // Gamestate ref to keep track of game speed modifiers

#pragma endregion

/** Attached components*/
#pragma region components
 public:

   /** Component that hangs over our head to visually denote how much health we have left*/
   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   UHealthbarComp* healthBar;

   /** Material reference to unit's base material*/
   UPROPERTY(EditAnywhere)
   UMaterialInterface* material;

   /** Selection circle decal below unit*/
   UPROPERTY(EditAnywhere)
   UDecalComponent* selectionCircleDecal;

   /** Sphere representing vision radius*/
   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   USphereComponent* visionSphere;

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
   void SetTarget(AUnit* value) { targetData.targetUnit = value; targetData.targetActor = value; }

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
   FORCEINLINE int GetAttributeBaseValue(int att) const { return baseC->GetAttribute(att)->GetBaseValue(); };

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetSkillBaseValue(int skill) const { return baseC->GetSkill(skill)->GetBaseValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetVitalBaseValue(int vit) const { return baseC->GetVital(vit)->GetBaseValue(); };

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetMechanicBaseValue(int mec) const { return baseC->GetMechanic(mec)->GetBaseValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE int GetAttributeAdjValue(int att) const { return baseC->GetAttribute(att)->GetCurrentValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetSkillAdjValue(int skill) const { return baseC->GetSkill(skill)->GetAdjustedValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetMechanicAdjValue(int mec) const { return baseC->GetMechanic(mec)->GetCurrentValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetVitalAdjValue(int vit) const { return baseC->GetVital(vit)->GetAdjustedValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetVitalCurValue(int vit) const { return baseC->GetVital(vit)->GetCurrValue(); }

   /**Only used to force hp to change to another value by triggers or cheatmenu*/
   UFUNCTION(BlueprintCallable, Category = "StatAccessors")
   void SetVitalCurValue(int vit, int vitValue) const { baseC->GetVital(vit)->SetCurrValue(vitValue); }

   /**Get Level of unit from baseCharacter*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE int GetLevel() const { return baseC->GetLevel(); }

   UFUNCTION(BlueprintCallable, Category = "Stats")
   void UpdateStats(); // call whenever stats get changed

   /**Allows us to apply some bonuses to this hero's stat
    * @param category - int representing what stat to modify
    * @param value - value to modify the stat by */
   UFUNCTION(BlueprintCallable, Category = "Helper")
   void ApplyBonuses(int category, int value);

#pragma endregion

#pragma endregion

 public:

   /** Function to find the bounds of a unit */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
   FBox2D FindBoundary();

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
   friend void UDamageCalculation::DamageTarget(AUnit* sourceUnit, AUnit* targetUnit, Damage& d, FGameplayTagContainer effects) const;

   /** Process to create damage effect */
   UFUNCTION(BlueprintNativeEvent)
   void Attack();
   virtual void Attack_Implementation();

 protected:

   UnitTargetData       targetData;
   UPAICombatParameters combatParams;

 public:

   UFUNCTION(BlueprintCallable)
   inline bool IsStunned() const;

   UFUNCTION(BlueprintCallable)
   inline bool IsSilenced() const;

   /**Create some text to display the damage dealt*/
   void ShowDamageDealt(Damage& damage);

   /**Overload of ShowDamageDealt incase we get a dodge or block perhaps*/
   void ShowDamageDealt(FText occurance);

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

   int  GetVisionCount() const { return enemyVisionCount; }
   void IncVisionCount() { ++enemyVisionCount; }
   void DecVisionCount() { --enemyVisionCount; }

   virtual bool IsVisible() PURE_VIRTUAL(AUnit::IsVisible, return false; ); 
   virtual TSet<AUnit*>* GetSeenEnemies() PURE_VIRTUAL(AUnit::GetSeenEnemies, return nullptr; );  

private:

   /** Counter for number of enemies (units with opposite value of isEnemy) that can see this unit */
   int enemyVisionCount;

   friend void USaveLoadClass::SetupBaseCharacter(AAlly* spawnedAlly, FBaseCharacterSaveInfo& baseCSaveInfo);

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
   static const float                   diminishParam;

 protected:
   static const TFunction<float(float)> diminishFunc;

 public:
   UFUNCTION(BlueprintCallable, BlueprintPure)
   float CalculateRisk();
   UFUNCTION(BlueprintCallable, BlueprintPure)
   float CalculateThreat();

   virtual float CalculateTargetRisk() PURE_VIRTUAL(AUnit::CalculateTargetRisk, return 0.f;);

#pragma endregion
};
