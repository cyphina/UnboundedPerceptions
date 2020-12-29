#pragma once

#include "AIController.h"
#include "UnitProperties.h"

#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "UnittargetData.h"

#include "GameplayTags.h"
#include "RTSAbilitySystemComponent.h"

#include "SaveLoadClass.h"
#include "SpellCastComponent.h"
#include "UnitCallbacks.h"
#include "UnitMessages.h"
#include "State/IUnitState.h"

#include "State/RTSStateMachine.h"

#include "WorldObject.h"
#include "Unit.generated.h"

class URTSDamageEffect;
class UpCombatInfo;
class UUpStatComponent;
class URTSVisionComponent;
class UTargetComponent;
class IAttackAnim;
struct FUpDamage;

DECLARE_STATS_GROUP(TEXT("RTSUnits"), STATGROUP_RTSUnits, STATCAT_Advanced);

/**
 * @brief Base class for all things in the world that fights!
 * - This class is a model/view class.
 * - Any components on this will be data related.
 * - Functionality on this class will be used to expose access to its data, but no manipulation
 */
UCLASS(Abstract, HideCategories = (Movement, Optimization, Character, Camera, CharacterMovement, Lighting, Rendering, Input, Actor))
class MYPROJECT_API AUnit : public ACharacter, public IWorldObject, public IAbilitySystemInterface, public IUnitCallbacks
{
   GENERATED_BODY()

   friend class AUnitController;
   friend void USaveLoadClass::SetupBaseCharacter(AAlly* spawnedAlly, FBaseCharacterSaveInfo& baseCSaveInfo);
   friend class IncantationState;
   friend class ChannelingState;
   friend class CastingState;
   friend class AttackState;
   friend class MovingState;
   friend class AttackMoveState;
   friend class ChasingState;

 public:
   AUnit(const FObjectInitializer& oI);

   /**
    * @brief Gets current state in state machine
    * @return Returns enum identifier corresponding to current state the state machine is in.
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   EUnitState GetState() const;

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
   FORCEINLINE virtual bool GetIsEnemy() const PURE_VIRTUAL(AUnit::GetIsEnemy, return false; );

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   FORCEINLINE bool GetIsDead() const;

   /**
    * Function to find the bounds of a unit (screen space points)
    * Used when we're creating our selection ring around a unit
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
   FBox2D FindBoundary() const;

   /**
    * Function to disable (pretty much erases its trace from the world) but not destroy this actor in memory
    * Removes heroes and allies from the hero and ally array, although there may be a way to get them back through resurrection
    */
   UFUNCTION(BlueprintCallable, Category = "Functionality")
   virtual void SetEnabled(bool bEnabled);

   URTSAbilitySystemComponent* GetAbilitySystemComponent() const override { return abilitySystemComponent; }
   URTSVisionComponent*        GetVisionComponent() const { return visionComponent; }
   UUpStatComponent*           GetStatComponent() const { return statComponent; }
   UTargetComponent*           GetTargetComponent() const { return targetComponent; }

   UpCombatInfo*           GetCombatInfo() const { return combatInfo.Get(); }

   FOnUnitDie&            OnUnitDie() const { return OnUnitDieEvent; }
   FOnUnitDamageReceived& OnUnitDamageReceived() const { return OnUnitDamageReceivedEvent; }
   FOnUnitDamageDealt&    OnUnitDamageDealt() const { return OnUnitDamageDealtEvent; }
   FOnUnitStop&           OnUnitStop() const { return OnUnitStopEvent; }
   FOnUnitHit&            OnUnitHit() const { return OnUnitHitEvent; }

 protected:
   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   class UHealthbarComp* healthBar;

   /** A little circle underneath the unit showing the radius of its collider */
   UPROPERTY(VisibleAnywhere)
   UDecalComponent* selectionCircleDecal;

   /** Units that don't have any vision can disable this or eventually we can refactor this to be optional*/
   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   URTSVisionComponent* visionComponent;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   UUpStatComponent* statComponent;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   UTargetComponent* targetComponent;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   URTSAbilitySystemComponent* abilitySystemComponent;

   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldObject Classification", meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
   FUnitProperties unitProperties;

   /**
    * @brief Material reference to unit's base material so that if it changes (due to effects) we can revert it back
    */
   UPROPERTY(EditAnywhere)
   UMaterialInterface* originalMaterial;

   void BeginPlay() override;
   void Tick(float deltaSeconds) override;
   void PossessedBy(AController* newController) override;

   class AUserInput* controllerRef;

 private:
   /** Change speed-based parameters when time multiplier changes */
   UFUNCTION(Category = "Callback")
   void OnUpdateGameSpeed(float speedMultiplier);

   void SetupHealthbarComponent();
   void SetupCharacterCollision() const;
   void SetupMovementComponent() const;
   void RemoveArrowComponent() const;

   void SetupAbilitiesAndStats();
   void AlignSelectionCircleWithGround() const;
   void StoreUnitHeight();

   class AUnitController* unitController = nullptr;

   // Reference to combat parameters kept in pointer since these values are barely used much (cold splitting)
   TUniquePtr<UpCombatInfo> combatInfo;

   mutable FOnUnitDie            OnUnitDieEvent;
   mutable FOnUnitDamageReceived OnUnitDamageReceivedEvent;
   mutable FOnUnitDamageDealt    OnUnitDamageDealtEvent;

   mutable FOnUnitStop OnUnitStopEvent;
   mutable FOnUnitHit  OnUnitHitEvent;
};
