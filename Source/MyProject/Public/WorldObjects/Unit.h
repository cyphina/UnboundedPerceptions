#pragma once

#include "WorldObject.h"
#include "GameFramework/Character.h"
#include "State/StateMachine.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "SpellSystem/Calcs/DamageCalculation.h"
#include "AIController.h" //Need this to add to delegate OnMoveCompleted
#include "GameplayAbilityTargetTypes.h"
#include "GameplayTags.h"
#include "Stats/BaseCharacter.h"
#include "SaveLoadClass.h"
#include "Unit.generated.h"

class	UMyGameInstance;
class	ARTSGameState;
class	AAIController;
class	StateMachine;
class	UDamageEffect;
class	UMyAbilitySystemComponent;
class	UMySpell;
class	UHealthbarComp;

UCLASS(abstract)
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
	float							height;
	TArray<FGameplayAttribute>		atts; //holds all the attributes in AbilityComponent.  We need this because we use the names because the effects use them, however getting them requires them being copied
	UMyGameInstance*				gameInstance;

protected:

	bool							isEnemy = false; //protected so we can set this in enemy class
	StateMachine					state = StateMachine(); //reference to statemachine
	TUniquePtr<FBaseCharacter>		baseC = nullptr; //Reference to statmanager.  Is a pointer so we can make it and give it a reference to the attribute set
	AAIController*					controller = nullptr; //reference to AIController
	ARTSGameState*					gameState = nullptr; //gamestate ref to keep track of game speed modifiers 

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

#pragma region Callbacks
	void							BeginPlay() override;

	void							Tick(float deltaSeconds) override;

	//Change speed based parameters when time multiplier changes
	UFUNCTION(Category = "Callback") 
	void							OnUpdateGameSpeed(float speedMultiplier); 

	//Function called when unit dies :(
	UFUNCTION(BlueprintCallable, Category = "Functionality") 
	virtual	void					Die(); 

private:
	//callback must be a UFUNCTION
	UFUNCTION() 
	void							OnMoveCompleted(FAIRequestID RequestID, const EPathFollowingResult::Type Result); //Callback function for move completed

#pragma endregion

//Accessing our private data members
#pragma region Accessors
public:
	IUnitState* GetState() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	FName							 GetStateName(); //Gets name of current state in statemachine

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	void							 SetGameName(FText value) final override { name = value; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	FText							 GetGameName() const final override { return name; } 

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	void							 SetImage(UTexture2D* value) final override { image = value; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	UTexture2D*						 GetImage() const final override { return image; }

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	void							 SetSelected(bool value) override { isSelected = value; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	bool							 GetSelected() const override { return isSelected; }

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	void							 SetObjectID(int value) final override { objectID = value; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	int								 GetObjectID() const final override { return objectID; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
	bool							 GetIsEnemy() const { return isEnemy; }

	UFUNCTION(BlueprintCallable, Category = "CombatAccessors")
	void							 SetCanTarget(bool value) override { canTarget = value; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
	bool							 GetCanTarget() const override { return canTarget;  }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
	bool							 GetIsDead() const  { return isDead; }

	const FBaseCharacter&			 GetBaseCharacter() const { return *baseC; }

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	TSubclassOf<UMySpell>			 GetCurrentSpell() const { return currentSpell; }

///--expose stats to blueprints and other units but only change the stats from C++ which is why we don't see any setters--
#pragma region Stats
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	int								 GetAttributeBaseValue(int att) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	float							 GetSkillBaseValue(int skill) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	float							 GetVitalBaseValue(int vit) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	float							 GetMechanicBaseValue(int mec) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	int								 GetAttributeAdjValue(int att) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	float							 GetSkillAdjValue(int skill) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	float							 GetMechanicAdjValue(int mec) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	float							 GetVitalAdjValue(int vit) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	float							 GetVitalCurValue(int vit) const;

	/**Only used to force hp to change to another value by triggers or cheatmenu*/
	UFUNCTION(BlueprintCallable, Category = "StatAccessors")
	void							 SetVitalCurValue(int vit, int vitValue) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
	int							     GetLevel() const; //Get Level of unit from BaseC

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void							 UpdateStats(); //call whenever stats get changed

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

	//Delete damage indicator after it falls 
	void							deleteDI();

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
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool							isDead; 

	//Get data on our spell target
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
	FGameplayAbilityTargetDataHandle	GetTargetData() const { return targetData; } 

	//Set data on our spell target
	UFUNCTION(BlueprintCallable, Category = "Spells")
	void								SetTarget(AUnit* target) { targetUnit = target; }

	//Process to create damage effect
	virtual void						Attack(); 

	//Damage Calculations on Receiving End.  If using some magic armor, attacker may get some debuffs
	friend void							UDamageCalculation::ReceiveDamage(AUnit* unit, Damage& damage, FGameplayTagContainer& container) const;

	//Remove the modifiers of some gameplayeffect to the unit when they expire
	//void								RemoveGameplayEffects(const FActiveGameplayEffect& effect);

	/**Preparations before we can actually execute an attack with our weapon like getting in range and turning the right way*/
	void								PrepareAttack(); // PURE_VIRTUAL(AUnit::PrepareAttack, );

protected:
	AUnit*								targetUnit; //used to target individual units (via spell cast or right click attack) because targetData has too much info
	FVector								targetLocation; //used to target area (no restrictions on what can be clicked)
	FGameplayAbilityTargetDataHandle	targetData = FGameplayAbilityTargetDataHandle(); //detailed spell targetting information

	float								currentAttTime = 0; //keeps track of how long we wait between autoattacks
	bool								readyToAttack = false; //bool to check if we can autoattack again
	inline bool							IsStunned() const; //are we currently stunned?
	inline bool							IsSilenced() const;
	FGameplayTag						combatStyle; //type of attack we autoattack with

public:
	//Create some text to display the damage dealt
	void								ShowDamageDealt(Damage& damage);

	//Overload of ShowDamageDealt incase we get a dodge or block perhaps
	void								ShowDamageDealt(FText occurance);

	//Apply the modifiers of some gameplayeffect to the unit
	//void								ApplyGameplayEffects(const FGameplayEffectSpec& effect);							
#pragma region Skills
public:
	//List of abilities that are in unit's skill slots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<class UMySpell>> abilities; 	

	//Function called when ability is activated to allow spell to drain resources.  Public because MySpell needs to reference it
	void								CommitCast(UMySpell* spell);

protected:
	//current spell attempting to be casted
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "Abilities")
	TSubclassOf<UMySpell>				currentSpell = nullptr; 

	//Actually activates the ability and triggers abilities that wait for click events to continue
	virtual bool						CastSpell(TSubclassOf<UMySpell> spellToCast);
	
	//Do we have the resources necessary to cast this spell
	bool								CanCast(TSubclassOf<UMySpell> spellToCheck); 

private:
	UMyAbilitySystemComponent*			abilitySystem; //Skill component

	//Adjust position for spell casting and check if stunned
	void								PrepareCastSpell();


	friend void							USaveLoadClass::SetupBaseCharacter(AAlly* spawnedAlly, FBaseCharacterSaveInfo& baseCSaveInfo);
#pragma endregion
#pragma endregion
};
