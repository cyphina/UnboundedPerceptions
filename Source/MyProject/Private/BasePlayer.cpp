#include "MyProject.h"
#include "BasePlayer.h"
#include "EngineUtils.h"
#include "GameplayDelegateContext.h"
#include "WorldObjects/BaseHero.h"
#include "Quests/QuestManager.h"
#include "GameplayTagContainer.h"
#include "PartyDelegateContext.h"
#include "RTSPawn.h"
#include "UIDelegateContext.h"

ABasePlayer::ABasePlayer()
{
   money = 1000;
}

void ABasePlayer::BeginPlay()
{
   Super::BeginPlay();
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnAllyActiveChanged().AddUObject(this, &ABasePlayer::OnAllyActiveChanged);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnSummonActiveChanged().AddUObject(this, &ABasePlayer::OnSummonActiveChanged);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnHeroActiveChanged().AddUObject(this, &ABasePlayer::OnHeroActiveChanged);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnEnemySelectedWithouDebugging().BindUObject(
   this, &ABasePlayer::SetFocusedUnit);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UUIDelegateContext>()->OnUnitSlotSelected().AddUObject(this, &ABasePlayer::OnUnitSlotSelected);
   Cast<ARTSPawn>(GetWorld()->GetFirstPlayerController()->GetPawn())->OnGroupTabbed().AddUObject(this, &ABasePlayer::OnGroupTabbed);
}

AUnit* ABasePlayer::GetFocusedUnit() const
{
   return focusedUnit ? focusedUnit : selectedUnits.Num() == 1 ? selectedUnits[0] : nullptr;
}

void ABasePlayer::SetFocusedUnit(AUnit* newFocusedUnit)
{
   focusedUnit = newFocusedUnit;
   if(ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController())
   {
      localPlayer->GetSubsystem<UPartyDelegateContext>()->OnFocusedUnitChanged().Broadcast(newFocusedUnit);
   }
}

void ABasePlayer::ClearSelectedUnits()
{
   for(AUnit* selectedUnit : selectedUnits)
   {
      selectedUnit->ClearSelectedFlag();
   }

   selectedUnits.Empty();
   selectedAllies.Empty();
   selectedHeroes.Empty();

   focusedUnit = nullptr;

   if(ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController())
   {
      localPlayer->GetSubsystem<UPartyDelegateContext>()->OnSelectionClearedDelegate.Broadcast();
   }
}

void ABasePlayer::UpdateActiveParty(TArray<ABaseHero*> newHeroes)
{
   if(!ensure(newHeroes.Num() > 0 && newHeroes.Num() <= MAX_NUM_HEROES))
   {
      UE_LOG(LogTemp, Error, TEXT("Inappropriate size (%d) of hero array"), newHeroes.Num());
      return;
   }

   while(heroes.Num() > 0)
   {
      heroes[0]->SetEnabled(false);
   }

   int i = -1;
   while(++i < newHeroes.Num())
   {
      newHeroes[i]->SetEnabled(true);
      newHeroes[i]->SetHeroIndex(i);
   }

   partyUpdatedEvent.Broadcast();
}

void ABasePlayer::AddHeroToRoster(ABaseHero* newHero)
{
   allHeroes.Add(newHero);
}

void ABasePlayer::UpdateGold(int32 amount)
{
   money = money + amount;
}

void ABasePlayer::UpdateEXP(int32 amount)
{
   for(ABaseHero* hero : heroes)
   {
      if(IsValid(hero))
      {
         hero->SetCurrentExp(amount);
      }
   }
}

void ABasePlayer::LearnDialogTopic(FGameplayTag topic)
{
   dialogTopics.AddTag(topic);
   OnDialogLearned.Broadcast(topic);
}

void ABasePlayer::SetMoney(int newMoneyVal)
{
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UGameplayDelegateContext>()->OnMoneyGained().Broadcast(newMoneyVal - newMoneyVal);
   money = newMoneyVal;
}

void ABasePlayer::OnHeroSelected(ABaseHero* heroRef)
{
   selectedHeroes.AddUnique(heroRef);
}

void ABasePlayer::OnHeroDeselected(ABaseHero* heroRef)
{
   selectedHeroes.RemoveSingle(heroRef);
}

void ABasePlayer::OnAllySelected(AAlly* allyRef)
{
   selectedAllies.AddUnique(allyRef);
}

void ABasePlayer::OnUnitSelected(AUnit* unitRef)
{
}

void ABasePlayer::OnAllyDeselected(AAlly* allyRef)
{
   selectedAllies.RemoveSingle(allyRef);
}

void ABasePlayer::OnUnitDeselected(AUnit* unitRef)
{
}

void ABasePlayer::OnUnitSlotSelected(AUnit* unitSelected)
{
   if(IsValid(unitSelected))
   {
      SetFocusedUnit(unitSelected);
   }
}

void ABasePlayer::OnGroupTabbed(AUnit* newFocusedUnit)
{
   if(newFocusedUnit)
   {
      SetFocusedUnit(newFocusedUnit);
   }
}

void ABasePlayer::OnAllyActiveChanged(AAlly* allyRef, bool isActive)
{
   if(isActive)
      allies.Add(allyRef);
   else
   {
      allies.RemoveSingle(allyRef);
      if(allyRef->GetUnitSelected()) { selectedAllies.RemoveSingle(allyRef); }
   }
}

void ABasePlayer::OnHeroActiveChanged(ABaseHero* heroRef, bool isActive)
{
   if(isActive)
   {
      heroes.Add(heroRef);
   } else
   {
      heroes.RemoveSingle(heroRef);
      for(int i = 0; i < heroes.Num(); ++i)
      {
         heroes[i]->SetHeroIndex(i);
      }
   }
}

void ABasePlayer::OnSummonActiveChanged(ASummon* summonRef, bool isActive)
{
   if(isActive)
      summons.Add(summonRef);
   else
      summons.RemoveSingle(summonRef);
}
