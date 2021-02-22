#pragma once

#include "AIController.h"
#include "UnitProperties.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RTSAbilitySystemComponent.h"
#include "SaveLoadClass.h"
#include "CombatParameters.h"
#include "WorldObject.h"
#include "Unit.generated.h"

class UWidgetComponent;
class URTSDamageEffect;
class UUpStatComponent;
class URTSVisionComponent;
class UTargetComponent;
class IAttackAnim;
class URTSAttackExecution;
class URTSDeathExecution;
class URTSMoveExecution;
struct UpCombatInfo;
struct FUpDamage;

DECLARE_STATS_GROUP(TEXT("RTSUnits"), STATGROUP_RTSUnits, STATCAT_Advanced);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitDamageReceived, const FUpDamage&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitDamageDealt, const FUpDamage&);
DECLARE_EVENT(AUnitController, FOnUnitDie);

DECLARE_EVENT(URTSUnitAnimController, FOnUnitAttackSwingHit); // When a unit initiates an attack (animation begins)

/**
 * @brief Base class for all things in the world that fights!
 * - This class is a model/view class.
 * - Any components on this will be data related.
 * - Functionality on this class will be used to expose access to its data, but no manipulation
 */
UCLASS(Abstract, HideCategories = (Movement, Optimization, Character, Camera, CharacterMovement, Lighting, Rendering, Input, Actor))
class MYPROJECT_API AUnit : public ACharacter, public IWorldObject, public IAbilitySystemInterface
{
   GENERATED_BODY()

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
   ~AUnit();

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
   virtual void SetUnitSelected(bool value);

   void ClearSelectedFlag() { unitProperties.isSelected = false; };

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   virtual bool GetUnitSelected() const { return unitProperties.isSelected; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   virtual bool GetIsEnemy() const PURE_VIRTUAL(AUnit::GetIsEnemy, return false;);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   bool GetIsDead() const;

   bool GetIsUnitHidden() const { return unitProperties.isUnitHidden; }
   void SetIsUnitHidden(bool isHidden) { unitProperties.isUnitHidden.AtomicSet(isHidden); }

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

   bool IsEnabled() const { return unitProperties.bIsEnabled; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   const TSet<AUnit*>& GetVisibleEnemies() const { return *GetVisibleEnemies_Impl(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   const TSet<AUnit*>& GetAllies() const { return *GetAllies_Impl(); }

   URTSAbilitySystemComponent* GetAbilitySystemComponent() const override { return abilitySystemComponent; }

   URTSVisionComponent* GetVisionComponent() const { return visionComponent; }

   UUpStatComponent* GetStatComponent() const { return statComponent; }

   UTargetComponent* GetTargetComponent() const { return targetComponent; }

   UpCombatInfo* GetCombatInfo() const { return combatInfo.Get(); }

   FOnUnitDie& OnUnitDie() const { return OnUnitDieEvent; }

   FOnUnitDamageReceived& OnUnitDamageReceived() const { return OnUnitDamageReceivedEvent; }

   FOnUnitDamageDealt& OnUnitDamageDealt() const { return OnUnitDamageDealtEvent; }

   FOnUnitAttackSwingHit& OnUnitAttackSwingHit() const { return OnUnitAttackSwingHitEvent; }

   TSubclassOf<URTSAttackExecution> GetCustomAttackLogic() const { return customAttackLogic; }

   TSubclassOf<URTSDeathExecution> GetCustomDeathLogic() const { return customDeathLogic; }

   TSubclassOf<URTSMoveExecution> GetCustomMoveLogic() const { return customMoveLogic; }

 protected:
   void BeginPlay() override;
   void Tick(float deltaSeconds) override;
   void PossessedBy(AController* newController) override;

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
   UWidgetComponent* damageIndicatorWidget;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   URTSAbilitySystemComponent* abilitySystemComponent;

   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldObject Classification", meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
   FUnitProperties unitProperties;

   /**
   * Holds logic for basic auto attacks. By default this requires a targeted attack component but some units have custom attacks which may not require them to undergo
   * the targeting procedure.
   */
   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<URTSAttackExecution> customAttackLogic;

   /** Holds logic for death */
   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<URTSDeathExecution> customDeathLogic;

   /** Holds custom move logic so that we add some logic for the AI to move in certain ways instead of just walking everywhere.
   * For instance, if we want our character to teleport towards its target -
   * we can cast a teleport spell if its off CD and if the target is far or something. 
   */
   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<URTSMoveExecution> customMoveLogic;

   /**
   * @brief Material reference to unit's base material so that if it changes (due to effects) we can revert it back
   */
   UPROPERTY(EditAnywhere)
   UMaterialInterface* originalMaterial;
   
   class AUserInput* controllerRef;

 private:
   /** Change speed-based parameters when time multiplier changes */
   UFUNCTION(Category = "Callback")
   void OnUpdateGameSpeed(float speedMultiplier);

   void SetupHealthbarComponent();
   void SetupCharacterCollision() const;
   void SetupMovementComponent() const;
   void SetupDamageInidicatorContainerWidget();
   void RemoveArrowComponent() const;

   void SetupAbilitiesAndStats();
   void AlignSelectionCircleWithGround() const;
   void StoreUnitHeight();

   virtual const TSet<AUnit*>* GetVisibleEnemies_Impl() const PURE_VIRTUAL(AUnit::GetVisibleEnemies, return nullptr;);

   virtual const TSet<AUnit*>* GetAllies_Impl() const PURE_VIRTUAL(AUnit::GetAllies, return nullptr;);

   class AUnitController* unitController = nullptr;

   // Reference to combat parameters kept in pointer since these values are barely used much (cold splitting)
   TUniquePtr<UpCombatInfo> combatInfo;

   mutable FOnUnitDie            OnUnitDieEvent;
   mutable FOnUnitDamageReceived OnUnitDamageReceivedEvent;
   mutable FOnUnitDamageDealt    OnUnitDamageDealtEvent;
   mutable FOnUnitAttackSwingHit OnUnitAttackSwingHitEvent;
};
