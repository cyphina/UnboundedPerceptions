// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "BasePlayer.h"
#include "RTSGameMode.h"
#include "RTSGameState.h"
#include "UserInput.h"
#include "RTSPawn.h"

#include "Items/EquipmentMenu.h"
#include "Items/HeroInventory.h"
#include "UI/HUDManager.h"

#include "BaseHero.h"
#include "State/HeroStateMachine.h"
#include "AIStuff/AIControllers/HeroAIController.h"

#include "Interactables/Interactable.h"

#include "Trigger.h"

#include "ItemManager.h"
#include "Items/Consumable.h"
#include "Items/Equip.h"
#include "Items/Equipment.h"

#include "AbilitySystemComponent.h"
#include "RTSIngameWidget.h"
#include "RTSSidebarWidget.h"
#include "SpellSystem/MySpell.h"
#include "SpellSystem/Spellbook.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "WorldObjects/NPC.h"

const float ABaseHero::nextExpMultiplier = 1.5f;

//---Object Fundamentals---
#pragma region Fundamentals
ABaseHero::ABaseHero(const FObjectInitializer& oI) : AAlly(oI)
{
   state = MakeUnique<HeroStateMachine>(this);
   ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> materialPC(TEXT("/Game/RTS_Tutorial/Materials/CelShade/LightSource"));
   if(materialPC.Object) {
      lightSource = materialPC.Object;
   }
}

// Called when the game starts or when spawned
void ABaseHero::BeginPlay()
{
   Super::BeginPlay();
   // Setup player reference

   player = Cast<ABasePlayer>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController()->PlayerState);
   player->allHeroes.Add(this);

   // Setup dynamic material instance and vector parameter references
   if(GetMesh() != nullptr) {
      material = GetMesh()->GetMaterial(0);
      dMat     = UMaterialInstanceDynamic::Create(material, this);
      if(material != nullptr) {
         FLinearColor initDirection;
#if UE_BUILD_DEBUG
         if(!material->GetVectorParameterValue("BodyColor", baseColor))
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Failed to get material color"));
         else
#endif
            currentColor = baseColor;
#if UE_BUILD_DEBUG
         if(!material->GetVectorParameterValue("InstDirectLight", initDirection))
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("Failed to get light direction for %s"), *name.ToString()));
#endif
         GetMesh()->SetMaterial(0, dMat);
      }
   }

   // Setup initial states
   currentInteractable = nullptr;

   // Set light direction parameters (of imitated sun)
   lightDirectionPV = GetWorld()->GetParameterCollectionInstance(lightSource);

   // Default backpack size is 40 items
   backpack = NewObject<UBackpack>(this, "Backpack");
   backpack->SetItemMax(40);

   // This is the size of the hero equipment array.  Could make this a class later
   equipment = NewObject<UEquipment>(this, "Equipment");
   equipment->OnEquipped.BindDynamic(this, &ABaseHero::OnEquipped);

#if UE_EDITOR
   for(int i = 0; i < 70; i++) {
      baseC->LevelUp();
   }
#endif

   // Load up any stored triggers that tried to activate on this object but it wasn't alive
   TArray<FTriggerData> savedTriggers;
   Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode())->GetTriggerManager()->GetTriggerRecords().MultiFind(*GetGameName().ToString(), savedTriggers);
   for(FTriggerData& finishedTriggerActivation : savedTriggers) {
      controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(finishedTriggerActivation);
   }

   // TODO: Add all the item abilities to the abilitycomponent
   for(FName& id : UItemManager::Get().GetAllConsumableIDs()) {
      TSubclassOf<UMySpell> itemAbilityClass = UItemManager::Get().GetConsumableInfo(id)->abilityClass;
      if(IsValid(itemAbilityClass))
         GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(itemAbilityClass.GetDefaultObject(), 1));
   }

   spellbook          = NewObject<USpellBook>(this, spellbookClass.Get());
   spellbook->heroRef = this;
   spellbook->Init();
}

// Called every frame
void ABaseHero::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
   // if (dMat)
   // CheckShadows();
}

void ABaseHero::EndPlay(EEndPlayReason::Type epr)
{
   Super::EndPlay(epr);
}

void ABaseHero::PossessedBy(AController* newController)
{
   Super::PossessedBy(newController);
   heroController = Cast<AHeroAIController>(GetController());
}

void ABaseHero::SetEnabled(bool bEnabled)
{
   Super::SetEnabled(bEnabled);
   if(bEnabled) {
      controllerRef->GetBasePlayer()->heroes.Add(this);
   } else {
      // Renumber the hero indices, then remove hero. To bring this hero back we need some tracking else the party must be reassigned
      TArray<ABaseHero*>& heroesArray = controllerRef->GetBasePlayer()->heroes;
      heroesArray.RemoveSingle(this); 
      for(int i = heroIndex; i < heroesArray.Num(); ++i) {
         heroesArray[i]->heroIndex -= 1;
      }    
   }
}

void ABaseHero::Die_Implementation()
{
   // Set typical unit die parameters, deselect unit if it is selected, remove references to it in the vision calculation set, and gameover if all heroes died
   Super::Die_Implementation();

   bool isAllDead = true;
   for(ABaseHero* hero : controllerRef->GetBasePlayer()->heroes) {
      if(!hero->combatParams.isDead) {
         isAllDead = false;
         break;
      }
   }
   if(isAllDead)
      controllerRef->GetGameMode()->GameOver();
}
#pragma endregion

#pragma region Acessors

int ABaseHero::GetCurrentExp() const
{
   return currentExp;
}

int ABaseHero::GetExpToLevel() const
{
   return expForLevel;
}

void ABaseHero::SetCurrentExp(int amount)
{
   currentExp += amount;
   while(currentExp >= expForLevel) {
      currentExp = currentExp - expForLevel;
      expForLevel *= nextExpMultiplier; //
      LevelUp();
   }
}

AActor* ABaseHero::GetCurrentInteractable() const
{
   return Cast<AActor>(currentInteractable);
}

TArray<int> ABaseHero::GetEquipment() const
{
   return equipment->GetEquips();
}

#pragma endregion

#pragma region BaseCStuff

void ABaseHero::LevelUp_Implementation()
{
   attPoints += 5;
   skillPoints += 1;
   baseC->LevelUp();
   onLevelUp.Broadcast();
}

void ABaseHero::ChangeAttribute(EAttributes att, bool isIncrementing)
{
   if(isIncrementing) {
      GetAbilitySystemComponent()->SetNumericAttributeBase(UMyAttributeSet::IndexAtts(static_cast<int>(att)), GetAttributeBaseValue(att) + 1);
      --attPoints;
      baseC->StatUpdate(UMyAttributeSet::IndexAtts(static_cast<int>(att)));
   } else {
      GetAbilitySystemComponent()->SetNumericAttributeBase(UMyAttributeSet::IndexAtts(static_cast<int>(att)), GetAttributeBaseValue(att) - 1);
      ++attPoints;
      baseC->StatUpdate(UMyAttributeSet::IndexAtts(static_cast<int>(att)));
   }
}
#pragma endregion

#pragma region actions

void ABaseHero::Equip(int inventorySlotWeEquip)
{
   int prevEquipInSlot = equipment->Equip(backpack->GetItem(inventorySlotWeEquip).id);
   backpack->RemoveItemAtSlot(inventorySlotWeEquip);

   if(prevEquipInSlot)
   {
      auto item{FMyItem(prevEquipInSlot)};
      if(!ensure(backpack->AddItem(item))) UE_LOG(LogTemp, Error, TEXT("Impossible case reached no space to swap equipment"));
   }

   if(controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Equipment))
      controllerRef->GetHUDManager()->GetEquipHUD()->Update();
   if(controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Inventory))
      controllerRef->GetHUDManager()->GetInventoryHUD()->LoadItems();
}

void ABaseHero::Unequip(int unequipSlot)
{
   // If we have an item in the slot we're trying to unequip
   if(equipment->GetEquips()[unequipSlot]) {
      // If there is space in the backpack
      if(backpack->Count() < backpack->GetItemMax()) {
         int itemID = equipment->GetEquips()[unequipSlot];
         auto item{FMyItem(itemID)};
         backpack->AddItem(item); // Space checked again here
         equipment->Unequip(unequipSlot);
      }
      else
         controllerRef->GetHUDManager()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("Equipment", "NotEnoughSpaceUnequip", "Not enough space to unequip!"));

      if(controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Equipment))
         controllerRef->GetHUDManager()->GetEquipHUD()->Update();
      if(controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Inventory))
         controllerRef->GetHUDManager()->GetInventoryHUD()->LoadItems();
   }
}

void ABaseHero::SwapEquip(int equipSlot1, int equipSlot2)
{
   equipment->SwapEquips(equipSlot1, equipSlot2);
   if(controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Equipment))
      controllerRef->GetHUDManager()->GetEquipHUD()->Update();
}

void ABaseHero::UseItem(int itemID)
{
   // TODO: removes first instance for now instead of slot clicked
   // Make sure the user hasn't dropped the item after beginning to use it
   if(backpack->FindItem(itemID) != INDEX_NONE) {
      if(UItemManager::Get().GetItemInfo(itemID)->itemType.GetTagName() != "Item.Consumeable.Utility.Reusable")
         backpack->RemoveItem(FMyItem(itemID));
      // Refresh inventory after to update visual changes
      if(controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Inventory))
         controllerRef->GetHUDManager()->GetInventoryHUD()->LoadItems();
   }
}

#pragma endregion

#pragma region equipment

void ABaseHero::OnEquipped(int equipID, bool isEquip)
{
   FEquipLookupRow* e = UItemManager::Get().GetEquipInfo(equipID);
   for(auto& x : e->stats.defaultAttributes)
      ModifyStats<true>(GetAttributeBaseValue(x.att) + x.defaultValue * (2 * isEquip - 1), x.att);

   for(auto& x : e->stats.defaultUnitScalingStats)
      ModifyStats<true>(GetSkillBaseValue(x.stat) + x.defaultValue * (2 * isEquip - 1), x.stat);

   for(auto& x : e->stats.defaultVitals)
      ModifyStats<true>(GetVitalBaseValue(x.vit) + x.defaultValue * (2 * isEquip - 1), x.vit);

   for(auto& x : e->stats.defaultMechanics)
      ModifyStats<true>(GetMechanicBaseValue(x.mech) + x.defaultValue * (2 * isEquip - 1), x.mech);
}

bool ABaseHero::CastSpell(TSubclassOf<UMySpell> spellToCast)
{
   // Do the same thing as casting a spell
   bool sucessfulCast = Super::CastSpell(spellToCast);

   // If the itemID is set, then we casted this spell by using an item
   if(currentItem && sucessfulCast)
      UseItem(currentItem);
   return sucessfulCast;
}

void ABaseHero::SetSelected(bool value)
{
   Super::SetSelected(value);
   if(value) {
      controllerRef->GetBasePlayer()->selectedHeroes.AddUnique(this);
   } else {
      controllerRef->GetBasePlayer()->selectedHeroes.RemoveSingle(this);
   }
}

void ABaseHero::CheckShadows()
{
   if(IsValid(lightDirectionPV) && IsValid(dMat)) {
      FLinearColor matParam;
      if(lightDirectionPV->GetVectorParameterValue("DirectLight", matParam)) {
         FHitResult            hitResult;
         FCollisionQueryParams CQP;
         // CQP.TraceTag = GetWorld()->DebugDrawTraceTag;
         CQP.AddIgnoredActor(this);

         lightVector = FVector(matParam);

         // Make this bool alpha to see if we're in shadow
         bool inShadow = GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), GetActorLocation() + lightVector * shadowRange, ECC_Visibility, CQP);
         // DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + lightVector * shadowRange, FColor::Red);

         FMath::FInterpTo(matParam.A, inShadow, GetWorld()->GetDeltaSeconds(), 5); // Slowly make the material transparent
         if(dMat->GetVectorParameterValue(FMaterialParameterInfo("InstDirectLight"), matParam))
            dMat->SetVectorParameterValue("InstDirectLight", FLinearColor(lightVector.X, lightVector.Y, lightVector.Z, inShadow));
         // LightDirectionPV->SetVectorParameterValue("DirectLight", FLinearColor(lightVector.X, lightVector.Y, lightVector.Z, inShadow))
      }
   }
}
