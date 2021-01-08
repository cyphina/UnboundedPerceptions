// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "SpellBook.h"

#include "UserInput.h"

#include "WorldObjects/BaseHero.h"
#include "AIStuff/AIControllers/HeroAIController.h"
#include "HUDManager.h"
#include "UI/UserWidgets/RTSIngameWidget.h"

#include "AbilitySystemComponent.h"
#include "MySpell.h"
#include "UpStatComponent.h"

bool USpellBook::IsLearnable(SpellNode sNode)
{
   for(SpellNode s : sNode.prereqSpellNodes) {
      if(!GetLearnedSpells().Contains(s.index)) return false;
   }
   return true;
}

USpellBook::USpellBook()
{
}

USpellBook* USpellBook::CreateSpellBook(ABaseHero* heroRef)
{
   USpellBook* spellbook = NewObject<USpellBook>(heroRef);
   spellbook->Init();
   spellbook->heroRef = heroRef;
   return spellbook;
}

TArray<int> USpellBook::GetLearnableSpells() const
{
   {
      TArray<int> spellIndices = TArray<int>();
      for(SpellNode sNode : learnableSpells) {
         spellIndices.Add(sNode.index);
      }
      return spellIndices;
   }
}

TArray<int> USpellBook::GetLearnedSpells() const
{
   TArray<int> spellIndices = TArray<int>();
   for(SpellNode sNode : learnedSpells) {
      spellIndices.Add(sNode.index);
   }
   return spellIndices;
}

TArray<int> USpellBook::GetUnknownSpells() const
{
   TArray<int> spellIndices = TArray<int>();
   for(SpellNode sNode : unknownSpells) {
      spellIndices.Add(sNode.index);
   }
   return spellIndices;
}

UMySpell* USpellBook::GetDefaultAbilityCopy(int spellIndex) const
{
   return availableSpells[spellIndex].GetDefaultObject();
}

void USpellBook::Init()
{
   if(heroRef) {
      cpcRef = Cast<AUserInput>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController());

      int i = 0;
      for(TSubclassOf<UMySpell> spell : availableSpells) {
         UMySpell* spellObject = spell.GetDefaultObject();
         if(spellObject) {
            heroRef->GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(spellObject, 1));
            int spellID = spellObject->spellDefaults.id;

            spellNodes.Add(spellID, SpellNode(i, TArray<SpellNode>(), TArray<SpellNode>(), spell));

            if(USpellDataManager::GetData().GetSpellInfo(spellID)->preReqs.Num() == 0) {
               learnableSpells.AddTail(spellNodes[spellID]);
            } else {
               unknownSpells.AddTail(spellNodes[spellID]);
            }
         }
         ++i;
      }

      // Setup connections
      for(SpellNode s : unknownSpells) {
         if(s.spellRef.GetDefaultObject()) // if valid spell class
         {
            // Loop through preReqs
            for(int preReqIndex : USpellDataManager::GetData().GetSpellInfo(s.spellRef.GetDefaultObject()->spellDefaults.id)->preReqs) {
               if(spellNodes.Contains(preReqIndex)) // check if key exists in case spellID isn't coded
               {
                  s.prereqSpellNodes.Add(spellNodes[preReqIndex]);   // let node know of nodes that point to it
                  spellNodes[preReqIndex].unlockedSpellNodes.Add(s); // let nodes that point to node know they are pointing to this node
               }
            }
         }
      }
   }
}

void USpellBook::Respec()
{
   // TODO: Implement this
}

bool USpellBook::LearnNewSpell(UMySpell* spellObject) {
   URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "LearnNewSpell", "Learned a new spell"));
   learnedSpells.AddTail(spellNodes[spellObject->spellDefaults.id]);
   OnSpellLearned().Broadcast(spellObject->GetClass());

   for(SpellNode s : spellNodes[spellObject->spellDefaults.id].unlockedSpellNodes) 
   {
      if(IsLearnable(s))
         learnableSpells.AddTail(s);
   }
   return true;
}

bool USpellBook::TryUpgradeSpellLevel(UMySpell* spellObject, FGameplayAbilitySpec* abilityInfo)
{
   if(!CheckIfSpellMaxLevel(abilityInfo, spellObject))
   {
      ++abilityInfo->Level;
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "Upgrade", "Upgraded Ability!"));
      heroRef->GetAbilitySystemComponent()->MarkAbilitySpecDirty(*abilityInfo);
      OnSpellUpgraded().Broadcast(spellObject->GetClass());
      return true;
   } else {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "UpgradeFailMax", "Ability Already at Max Level!"));
   }
   return false;
}

bool USpellBook::LearnSpell(int index)
{
   check(index >= 0 && index < availableSpells.Num());
   UMySpell* spellObject = availableSpells[index].GetDefaultObject();

   if(spellObject) {
      if(!CheckLevel(spellObject)) return false;
      if(!CheckPoints()) return false;
      if(!CheckPrereqs(spellObject)) return false;
 
      FGameplayAbilitySpec* abilityInfo = heroRef->GetAbilitySystemComponent()->FindAbilitySpecFromClass(spellNodes[spellObject->spellDefaults.id].spellRef);
      
      if(!GetLearnedSpells().Contains(index)) 
      {
         return LearnNewSpell(spellObject);
      } else
      {
         return TryUpgradeSpellLevel(spellObject, abilityInfo);
      }
   }
   return false;
}

bool USpellBook::CheckLevel(UMySpell* spellObject) const
{
   if(heroRef->GetStatComponent()->GetUnitLevel() < spellObject->GetReqLevel(heroRef->GetAbilitySystemComponent())) {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(),
                                              NSLOCTEXT("Spellbook", "LevelRequirementNotMet", "You are not high enough level to learn this spell"));
      return false;
   }
   return true;
}

bool USpellBook::CheckPoints() const
{
   if(heroRef->GetSkillPoints() <= 0) {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "OutOfPoints", "Need more skill points!"));
      return false;
   }
   return true;
}

bool USpellBook::CheckPrereqs(UMySpell* spellObject) const
{
   for(int i : spellObject->GetPreReqs()) {
      if(!GetLearnableSpells().Contains(i)) {
         URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "PrereqsNotMet", "You're missing some prerequisite skill(s)"));
         return false;
      }
   }
   return true;
}

bool USpellBook::CheckIfSpellMaxLevel(FGameplayAbilitySpec* abilityInfo, UMySpell* spellObject) const
{
   return abilityInfo->Level >
            USpellDataManager::GetData().GetSpellInfo(spellObject->spellDefaults.id)->maxLevel;
}

void USpellBook::Update(int index)
{
}

void USpellBook::UpgradeSpell()
{
}
