#pragma once

#include "AIController.h"
#include "UnitProperties.h"

#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "UnittargetData.h"

#include "GameplayTags.h"
#include "RTSAbilitySystemComponent.h"
#include "Components/RTSVisionComponent.h"

#include "SaveLoadClass.h"
#include "SpellCastComponent.h"
#include "UnitCallbacks.h"
#include "UnitMessages.h"
#include "State/IUnitState.h"

#include "State/RTSStateMachine.h"
#include "Stats/BaseCharacter.h"

#include "WorldObject.h"
#include "Unit.generated.h"

class URTSDamageEffect;
class UPAICombatParameters;
class IAttackAnim;
struct FUpDamage;

DECLARE_STATS_GROUP(TEXT("RTSUnits"), STATGROUP_RTSUnits, STATCAT_Advanced);

/** Base class for all things in the world that fights!
 * This class is a model/view class.
 * Any components on this will be data related.
 * Functionality on this class will be used to expose access to its dsata, but no manipulation
 */
UCLASS(Abstract, HideCategories = (Movement, Optimization, Character, Camera, CharacterMovement, Lighting, Rendering, Input, Actor))
class MYPROJECT_API AUnit : public ACharacter, public IWorldObject, public IAbilitySystemInterface, public IUnitCallbacks
{
 private:
   GENERATED_BODY()

   friend class AUnitController;

 public:
   AUnit(const FObjectInitializer& oI);

   /** Messages we can send to the controller to let it know something happened*/
   static const UnitMessages AI_MESSAGES;

#pragma region Components
   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   class UHealthbarComp* healthBar;

   /** A little circle underneath the unit showing the radius of its collider*/
   UPROPERTY(VisibleAnywhere)
   UDecalComponent* selectionCircleDecal;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   class URTSVisionComponent* visionComponent;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   class UUpStatComponent* statComponent;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   class UTargetComponent* targetComponent;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   class URTSAbilitySystemComponent* abilitySystemComponent;

   URTSAbilitySystemComponent* GetAbilitySystemComponent() const override { return abilitySystemComponent; }
#pragma endregion

/** References used by unit types*/
#pragma region UnitInfoAndReferences

   TUniquePtr<IAttackAnim> attackAnim;

 protected:
   /** Struct holding some editable unit properties */
   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldObject Classification", meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
   FUnitProperties unitProperties;

   class AUserInput* controllerRef;

   TUniquePtr<RTSStateMachine> state = nullptr;

   /** Material reference to unit's base material so that if it changes (due to effects) we can revert it back */
   UPROPERTY(EditAnywhere)
   UMaterialInterface* material;

   // Reference to AIController
   UPROPERTY()
   class AUnitController* unitController = nullptr;

#pragma endregion

#pragma region Callbacks

   void BeginPlay() override;
   void Tick(float deltaSeconds) override;
   void PossessedBy(AController* newController) override;

   /**Change speed based parameters when time multiplier changes*/
   UFUNCTION(Category = "Callback")
   void OnUpdateGameSpeed(float speedMultiplier);

 public:
   FOnUnitDie&            OnUnitDie() override { return OnUnitDieEvent; };
   FOnUnitDamageReceived& OnUnitDamageReceived() override { return OnUnitDamageReceivedEvent; };
   FOnUnitDamageDealt&    OnUnitDamageDealt() override { return OnUnitDamageDealtEvent; };
   FOnUnitStop&           OnUnitStop() override { return OnUnitStopEvent; };
   FOnUnitHit&            OnUnitHit() override { return OnUnitHitEvent; };

#pragma endregion

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
   FORCEINLINE bool GetIsEnemy() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   FORCEINLINE bool GetIsDead() const;

#pragma endregion

 public:
   /** Function to find the bounds of a unit (screen space points)
    * Used when we're creating our selection ring around a unit
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
   FBox2D FindBoundary() const;

   /** Function to disable (pretty much erases its trace from the world) but not destroy this actor in memory
    * Removes heroes and allies from the hero and ally array, although there may be a way to get them back through resurrection
    */
   UFUNCTION(BlueprintCallable, Category = "Functionality")
   virtual void SetEnabled(bool bEnabled);

   /** Damage Calculations on Receiving End.  If using some magic armor, attacker may get some debuffs */
   friend bool  USpellCastComponent::CastSpell(TSubclassOf<class UMySpell> spellToCast);
   friend float USpellCastComponent::GetCurrentChannelTime() const;

 protected:
   UPROPERTY(EditAnywhere)
   TSubclassOf<IAttackAnim> attackAnimClass = nullptr;

   // Reference to combat parameters kept in pointer since these values are barely used much (cold splitting)
   TUniquePtr<UPAICombatParameters> combatParams;

 private:
   friend void USaveLoadClass::SetupBaseCharacter(AAlly* spawnedAlly, FBaseCharacterSaveInfo& baseCSaveInfo);
   friend class IncantationState;
   friend class ChannelingState;
   friend class CastingState;
   friend class AttackState;
   friend class MovingState;
   friend class AttackMoveState;
   friend class ChasingState;

   FOnUnitDie            OnUnitDieEvent;
   FOnUnitDamageReceived OnUnitDamageReceivedEvent;
   FOnUnitDamageDealt    OnUnitDamageDealtEvent;

   FOnUnitStop OnUnitStopEvent;
   FOnUnitHit  OnUnitHitEvent;

   void SetupHealthbarComponent();
   void SetupCharacterCollision() const;
   void SetupMovementComponent() const;
   void RemoveArrowComponent() const;

   void SetupAbilitiesAndStats();
   void AlignSelectionCircleWithGround() const;
   void StoreUnitHeight();
};
