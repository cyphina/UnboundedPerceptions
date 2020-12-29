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
   spellbook->heroRef = &heroRef;
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
}

bool USpellBook::LearnSpell(int index)
{
   check(index >= 0 && index < availableSpells.Num());
   UMySpell* spellObject = availableSpells[index].GetDefaultObject();
   // if we haven't learned this spell yet

   if(spellObject) {
      if(heroRef->GetSkillPoints() <= 0) {
         cpcRef->GetWidgetProvider()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("Spellbook", "OutOfPoints", "Need more skill points!"));
         return false;
      }

      if(heroRef->GetStatComponent()->GetUnitLevel() < spellObject->GetReqLevel(heroRef->GetAbilitySystemComponent())) {
         cpcRef->GetWidgetProvider()->GetIngameHUD()->DisplayHelpText(
             NSLOCTEXT("Spellbook", "LevelRequirementNotMet", "You are not high enough level to learn this spell"));
         return false;
      }

      for(int i : spellObject->GetPreReqs()) // if we have the prereqs go on
      {
         if(!GetLearnableSpells().Contains(i)) {
            cpcRef->GetWidgetProvider()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("Spellbook", "PrereqsNotMet", "You're missing some prerequisite skill(s)"));
            return false;
         }
      }

      FGameplayAbilitySpec* abilityInfo = heroRef->GetAbilitySystemComponent()->FindAbilitySpecFromClass(spellNodes[spellObject->spellDefaults.id].spellRef);
      if(!GetLearnedSpells().Contains(index)) // this spell has not been learned yet
      {
         cpcRef->GetWidgetProvider()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("Spellbook", "LearnNewSpell", "Learned a new spell"));
         learnedSpells.AddTail(spellNodes[spellObject->spellDefaults.id]);
         --heroRef->skillPoints;
         // Don't have to increment level since we start off abilities at level 1 anyways
         for(SpellNode s : spellNodes[spellObject->spellDefaults.id].unlockedSpellNodes) // if this spell unlocked new ones, then add those new ones to the learnable list
         {
            if(IsLearnable(s)) learnableSpells.AddTail(s);
         }
         return true;
      } else // if we already learned this spell, level it up
      {
         if(abilityInfo->Level <
            USpellDataManager::GetData().GetSpellInfo(availableSpells[index].GetDefaultObject()->spellDefaults.id)->maxLevel) // if we aren't at the max level already
         {
            ++abilityInfo->Level; // increment the level by 1
            cpcRef->GetWidgetProvider()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("Spellbook", "Upgrade", "Upgraded Ability!"));
            heroRef->GetAbilitySystemComponent()->MarkAbilitySpecDirty(*abilityInfo);
            --heroRef->skillPoints;
            return true;
         } else
            cpcRef->GetWidgetProvider()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("Spellbook", "UpgradeFailMax", "Ability Already at Max Level!"));
      }
   }
   return false;
}

void USpellBook::Update(int index)
{
}

void USpellBook::UpgradeSpell()
{
}
