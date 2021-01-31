#include "MyProject.h"
#include "AIStuff/AIControllers/EnemyAIController.h"

#include "RTSGameMode.h"
#include "RTSGameState.h"

#include "UserInput.h"
#include "BasePlayer.h"

#include "Quests/QuestManager.h"

#include "Enemy.h"

#include "RTSVisionComponent.h"
#include "SpellSystem/MySpell.h"

#include "SpellSystem/RTSAbilitySystemComponent.h"
#include "DIRender.h"

#include "NavArea_EnemySpot.h"
#include "PartyDelegateContext.h"
#include "TargetComponent.h"
#include "../BaseHero.h"

#include "UpStatComponent.h"

#include "Interactables/Pickup.h"

AEnemy::AEnemy(const FObjectInitializer& oI) : AUnit(oI)
{
   aggroRange = 20;

   GetCapsuleComponent()->AreaClass = UNavArea_EnemySpot::StaticClass(); //Custom area class so navigation filter for defensive movement will avoid this
   GetCapsuleComponent()->SetCollisionProfileName("Enemy");

   visionComponent->SetCollisionProfileName("EnemyVision");
   visionComponent->SetCollisionResponseToChannel(FRIENDLY_CHANNEL, ECR_Overlap); //Friendly
   visionComponent->SetCanEverAffectNavigation(true);
   visionComponent->AreaClass = UNavArea_EnemySpot::StaticClass();
   visionComponent->SetAbsolute(false, false, true);
   visionComponent->bDynamicObstacle = true;

   GetMesh()->CustomDepthStencilValue = 254;
}

const TSet<AUnit*>* AEnemy::GetVisibleEnemies_Impl() const
{
   const auto& gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
   return &gameStateRef->GetVisiblePlayerUnits();
}

const TSet<AUnit*>* AEnemy::GetAllies_Impl() const
{
   const auto& gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
   return &gameStateRef->GetAllEnemyUnits();
}

void AEnemy::BeginPlay()
{
   Super::BeginPlay();
   auto gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());

   gameStateRef->RegisterEnemyUnit(this);

   // Setup status as customized in level editor
   InitializeStats();
   SetActorHiddenInGame(true); //Set hidden by default so won't be revealed by vision
   OnUnitDie().AddUObject(this, &AEnemy::GiveRewardsOnDeath);
}

void AEnemy::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
}

void AEnemy::EndPlay(const EEndPlayReason::Type e)
{
   Super::EndPlay(e);
   SetEnabled(false);
}

void AEnemy::Destroyed()
{
   Super::Destroyed();
}

void AEnemy::SetUnitSelected(bool value)
{
   if(value) {
      controllerRef->GetBasePlayer()->SetFocusedUnit(this);
   } else {
      if(controllerRef->GetBasePlayer()->GetFocusedUnit() == this) controllerRef->GetBasePlayer()->SetFocusedUnit(nullptr);
   }
   // Call on unit selected delegate(s) afterwards
   Super::SetUnitSelected(value);
}

void AEnemy::SetEnabled(bool bEnabled)
{
   Super::SetEnabled(bEnabled);
   if(UPartyDelegateContext* store = Cast<ULocalPlayer>(controllerRef->Player)->GetSubsystem<UPartyDelegateContext>()) {
      store->OnEnemyActiveChanged().Broadcast(this, bEnabled);
   }

   if(bEnabled) {
      controllerRef->GetGameState()->RegisterEnemyUnit(this);
   } else {
      controllerRef->GetGameState()->UnRegisterEnemyUnit(this);
   }
}

void AEnemy::SpawnItemDrops()
{
   controllerRef->GetBasePlayer()->UpdateEXP(expGiven);
   controllerRef->GetBasePlayer()->UpdateGold(moneyGiven);

   for(FItemDrop& itemDrop : itemDrops) {
      const int dropRoll = FMath::FRandRange(0, 100);
      if(itemDrop.dropPerc > dropRoll) {
         APickup* itemPickup = GetWorld()->SpawnActorDeferred<APickup>(APickup::StaticClass(), GetActorTransform());
         itemPickup->item    = itemDrop.itemInfo;
         UGameplayStatics::FinishSpawningActor(itemPickup, GetActorTransform());
      }
   }
}

void AEnemy::GiveRewardsOnDeath()
{
   SpawnItemDrops();

   // Enemy is hidden in Unit's implementation but reveal damage numbers still
   TArray<UDIRender*> damageComponents;
   GetComponents<UDIRender>(damageComponents);
   for(auto x : damageComponents) {
      x->SetVisibility(true);
   }

   // Set alive to show damage number
   SetLifeSpan(2.f);
}

void AEnemy::InitializeStats()
{
   int index = -1;
   for(auto& x : initialStats.defaultAttributes) {
      statComponent->ModifyStats<true>(x.defaultValue, x.att);
      statComponent->ModifyStats<true>(x.defaultValue, x.att);
   }

   for(auto& x : initialStats.defaultUnitScalingStats) {
      statComponent->ModifyStats<true>(x.defaultValue, x.stat);
      statComponent->ModifyStats<true>(x.defaultValue, x.stat);
   }

   for(auto& x : initialStats.defaultVitals) {
      statComponent->ModifyStats<true>(x.defaultValue, x.vit);
      statComponent->ModifyStats<true>(x.defaultValue, x.vit);
   }

   for(auto& x : initialStats.defaultMechanics) {
      statComponent->ModifyStats<true>(x.defaultValue, x.mech);
      statComponent->ModifyStats<true>(x.defaultValue, x.mech);
   }
}
