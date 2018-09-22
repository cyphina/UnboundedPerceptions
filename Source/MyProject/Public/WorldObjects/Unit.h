#pragma once

#include "WorldObject.h"
#include "GameFramework/Character.h"
#include "State/StateMachine.h"
#include "State/IUnitState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "SpellSystem/Calcs/DamageCalculation.h"
#include "AIController.h" //Need this to add to delegate OnMoveCompleted
#include "UnitTargetData.h"
#include "Stats/BaseCharacter.h"
#include "GameplayTags.h"
#include "SaveLoadClass.h"
#include "CombatParameters.h"
#include "SpellSystem/UnitSpellData.h"
#include "Unit.generated.h"

class	AUserInput;
class	ARTSGameState;
class	AAIController;
class	UDamageEffect;
class	UHealthbarComp;

UCLASS(Abstract)
class MYPROJECT_API AUnit : public ACharacter, public IWorldObject, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AUnit(const FObjectInitializer& oI);
	
/*Information about this unit and important references/components*/
#pragma region UnitInfoAndReferences
private:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldObject Classification", meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
	FText							name;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldObject Classification", meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
	UTexture2D*						image = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldObject Classification", meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
	bool							canTarget = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
	int								visionRadius = 1000;
	float							height;
	bool							isSelected = false;

	UPAICombatParameters			combatParams; //records combat stats to determine some AI parameters

protected:

	bool							isEnemy = false; //protected so we can set this in enemy class
	TUniquePtr<StateMachine>		state = nullptr; //reference to statemachine
	TUniquePtr<FBaseCharacter>		baseC = nullptr; //Reference to statmanager.  Is a pointer so we can make it and give it a reference to the attribute set
	AAIController*					controller = nullptr; //reference to AIController
	ARTSGameState*					gameState = nullptr; //gamestate ref to keep track of game speed modifiers 
	UPROPERTY(BlueprintReadOnly, Meta=(AllowPrivateAccess="true"))
	AUserInput*						controllerRef = nullptr;

#pragma endregion

public:

#if UE_EDITOR
	bool							godMode = false;
#endif

	/*--DECOR--*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UHealthbarComp*					healthBar;

	/**material reference to unit's base material*/
	UPROPERTY(EditAnywhere)
	UMaterialInterface*				material;

	/**selection circle decal below unit*/
	UPROPERTY(EditAnywhere)
	UDecalComponent*				selectionCircleDecal;

	/**sphere representing vision radius*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USphereComponent*				visionSphere;

#pragma region Callbacks
	void							BeginPlay() override;
	void							Tick(float deltaSeconds) override;

	//Change speed based parameters when time multiplier changes
	UFUNCTION(Category = "Callback") 
	void							OnUpdateGameSpeed(float speedMultiplier); 

	/**Function called when unit dies :(*/
	UFUNCTION(BlueprintCallable, Category = "Functionality") 
	virtual	void					Die(); 

	/**Function to disable to not destroy this actor*/
	UFUNCTION(BlueprintCallable, Category = "Functionality")
	void							SetEnabled(bool bEnabled);

private:
	//callback must be a UFUNCTION
	UFUNCTION() 
	void							OnMoveCompleted(FAIRequestID RequestID, const EPathFollowingResult::Type Result); //Callback function for move completed

#pragma endregion

//Accessing our private data members
#pragma region Accessors
public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	EUnitState						 GetState() const; //Gets name of current state in statemachine

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	void							 SetGameName(FText value) final override { name = value; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	FText							 GetGameName() const final override { return name; } 

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	void							 SetImage(UTexture2D* value) { image = value; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	UTexture2D*						 GetImage() const { return image; }

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	virtual void					 SetSelected(bool value) { isSelected = value; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	virtual bool					 GetSelected() const { return isSelected; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
	bool							 GetIsEnemy() const { return isEnemy; }

	UFUNCTION(BlueprintCallable, Category = "CombatAccessors")
	void							 SetCanTarget(bool value) { canTarget = value; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
	bool							 GetCanTarget() const { return canTarget;  }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
	bool							 GetIsDead() const  { return combatParams.isDead; }

	const FBaseCharacter&			GetBaseCharacter() const { return *baseC; }

///--expose stats to blueprints and other units but only change the stats from C++ which is why we don't see any setters--
#pragma region Stats

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	FORCEINLINE int					 GetAttributeBaseValue(int att) const { return baseC->GetAttribute(att)->GetBaseValue(); };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	FORCEINLINE float				 GetSkillBaseValue(int skill) const { return baseC->GetSkill(skill)->GetBaseValue(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	FORCEINLINE float				 GetVitalBaseValue(int vit) const { return baseC->GetVital(vit)->GetBaseValue(); };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	FORCEINLINE float				 GetMechanicBaseValue(int mec) const { return baseC->GetMechanic(mec)->GetBaseValue(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	FORCEINLINE int					 GetAttributeAdjValue(int att) const { return baseC->GetAttribute(att)->GetCurrentValue(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	FORCEINLINE float				 GetSkillAdjValue(int skill) const { return baseC->GetSkill(skill)->GetAdjustedValue(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	FORCEINLINE float				 GetMechanicAdjValue(int mec) const { return baseC->GetMechanic(mec)->GetCurrentValue(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	FORCEINLINE float				 GetVitalAdjValue(int vit) const { return baseC->GetVital(vit)->GetAdjustedValue(); }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	FORCEINLINE float				 GetVitalCurValue(int vit) const { return baseC->GetVital(vit)->GetCurrValue(); }

	/**Only used to force hp to change to another value by triggers or cheatmenu*/
	UFUNCTION(BlueprintCallable, Category = "StatAccessors")
	void							 SetVitalCurValue(int vit, int vitValue) const { baseC->GetVital(vit)->SetCurrValue(vitValue); }

	/**
	 *Get Level of unit from baseCharacter
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	FORCEINLINE int					 GetLevel() const { return baseC->GetLevel(); }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void							 UpdateStats(); //call whenever stats get changed

	/**
	 * Allows us to apply some bonuses to this hero's stat
	 * @param hero - Hero to apply bonus to
	 * @param category - int representing what stat to modify
	 * @param value - value to modify the stat by
	 */
	UFUNCTION(BlueprintCallable, Category = "Helper")
	void					ApplyBonuses(int category, int value);
#pragma endregion

#pragma endregion

///---Helper functions and members---///
#pragma region utility
	//Used to get bounds of some object with collision (represents the corners of a cube)
	const FVector					 BoundsPointMapping[8] =
	{
		FVector(1.f, 1.f, 1.f),
		FVector(1.f, 1.f, -1.f),
		FVector(1.f, -1.f, 1.f),
		FVector(1.f, -1.f, -1.f),
		FVector(-1.f, 1.f, 1.f),
		FVector(-1.f, 1.f, -1.f),
		FVector(-1.f, -1.f, 1.f),
		FVector(-1.f, -1.f, -1.f)
	};

public:
	//Function for checking if a unit is in range of some action
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool							IsTargetInRange(float range, FVector targetLocation);

	//Function to see if unit is facing the direction
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool							IsFacingTarget(FVector targetLocation);

	//Function to turn self towards a direction
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void							TurnTowardsTarget(FVector targetLocation);

	//Function to move to appropriate distance from target and face direction
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool							AdjustPosition(float range, FVector targetLocation);

	//Function to find the bounds of a unit
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	FBox2D							FindBoundary();
#pragma endregion

///---Actions that trigger a change in unit state---///
#pragma region actions 

	//Function for moving units around, based upon the ACharacter move.  Changing our state is pretty much the only reason we have a custom move
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void					Move(FVector newLocation);

	//Stop should be overidden based on the subclass because stopping some classes has to cancel more things
	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual void					Stop(); 

	/**Used to initiate an attack on a target*/
	UFUNCTION(BlueprintCallable, Category = "Combat") 
	void							BeginAttack(AUnit* target);

	/**Initiate spell casting procedure without any input triggers.  Used for AI spellcasting*/
	UFUNCTION(BlueprintCallable, Category = "Spells")
	virtual bool					BeginCastSpell(int spellToCastIndex, FGameplayAbilityTargetDataHandle targetData);

#pragma endregion		

#pragma region combat
	/**
	 *Get data on our spell target.  Is reset after the spell is sucessfully casted 
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
	FGameplayAbilityTargetDataHandle	GetTargetData() const { return targetData.spellTargetData; } 

	/**
	 *Get data on targetted unit.  Used in single target spellcasting and attacking.  Reset after spell casted and if we attack a new target or stop. 
	 *Faster to use this then cast GetTargetData() to some unit type
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
	AUnit*								GetTargetUnit() const { return targetData.targetUnit; } 

	//Process to create damage effect
	virtual void						Attack(); 

	//Damage Calculations on Receiving End.  If using some magic armor, attacker may get some debuffs
	friend void							UDamageCalculation::DamageTarget(AUnit* sourceUnit, AUnit* targetUnit, Damage& d, FGameplayTagContainer effects) const;

	/**
	 *Preparations before we can actually execute an attack with our weapon like getting in range and turning the right way
	 */
	void								PrepareAttack(); // PURE_VIRTUAL(AUnit::PrepareAttack, );

protected:

	UnitTargetData						targetData;

	inline bool							IsStunned() const; //are we currently stunned?
	inline bool							IsSilenced() const;

public:
	/**
	 *Create some text to display the damage dealt
	 */
	void								ShowDamageDealt(Damage& damage);

	/**
	 *Overload of ShowDamageDealt incase we get a dodge or block perhaps
	 */
	void								ShowDamageDealt(FText occurance);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	float								GetDPS(float timespan);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	float								GetDamageRecievedPerSecond(float timespan);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	float								GetHealingPerSecond(float timespan);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	float								GetHealingRecievedPerSecond(float timespan);
	//float								GetStunPerSecond(float timespan);

	void								CalculateRisk();
	void								CalculateThreat();	
	
	inline UAbilitySystemComponent*		GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	FORCEINLINE TSubclassOf<UMySpell>	GetCurrentSpell() const { return unitSpellData.currentSpell; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	float								GetChannelTime() const { return unitSpellData.channelTime; } 

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	float								GetCurrentChannelTime() const { return unitSpellData.currentChannelTime; } 

///---Skills---
#pragma region Skills
public:
	//List of abilities that are in unit's skill slots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<class UMySpell>> abilities; 	

	//Gets CDO of any spell.  CDOs of objects can only be given a struct, this function lets us get the CDO as the type of the object.  Also checks to see if hero has spell, else it won't work
	//Trying to get instances only returns active instances, that is, spells that are marked "Active" and haven't been committed because they are on pause.  Use GetSpellInstance to get actual instance
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells") 
	UMySpell*							GetSpellCDO(TSubclassOf<UMySpell> spellClass) const;

	//Function called when ability is activated to allow spell to drain resources.  Public because MySpell needs to reference it.  
	void								CommitCast(UMySpell* spell);

	//Do we have the resources necessary to cast this spell
	bool								CanCast(TSubclassOf<UMySpell> spellToCheck); 

protected:

	inline void							SetCurrentSpell(TSubclassOf<UMySpell> newCurrentSpell) { unitSpellData.currentSpell = newCurrentSpell; }

	/**
	 * Actually activates the ability and triggers abilities that wait for click events to continue
	 * @param spellToCast - Spell we want to cast.  Left as a parameter because we can cast spells that aren't our current spells in some scenarios like item usage
	 */
	virtual bool						CastSpell(TSubclassOf<UMySpell> spellToCast);

	/**Checks to see if spell has a cast time, and if so, it will start channeling process.  Else it will just cast the spell*/
	void								PreCastChannelingCheck(TSubclassOf<UMySpell> spellToCast);

private:

	UnitSpellData						unitSpellData;

	//Adjust position for spell casting and check if stunned
	void								PrepareCastSpell();

#pragma endregion

///---Vision---
public:

	int									GetVisionCount() const { return enemyVisionCount; }
	void								IncVisionCount() { ++enemyVisionCount; }
	void								DecVisionCount() { --enemyVisionCount;}

private:
	/**
	 *Counter for number of enemies (units with opposite value of isEnemy) that can see this unit
	 */
	int									enemyVisionCount;

	friend void							USaveLoadClass::SetupBaseCharacter(AAlly* spawnedAlly, FBaseCharacterSaveInfo& baseCSaveInfo);
	friend 								ChannelingState;
	friend void							CastingState::Update(AUnit& unit, float deltaSeconds);
	friend void							AttackState::Update(AUnit& unit, float deltaSeconds);

#pragma endregion
};
