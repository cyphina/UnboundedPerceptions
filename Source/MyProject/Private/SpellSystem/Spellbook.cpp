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

bool USpellBook::IsLearnable(SpellNode sNode)
{
   for(SpellNode s : sNode.prevSpellNodes) {
      if(!GetLearnedSpells().Contains(s.index))
         return false;
   }
   return true;
}

USpellBook::USpellBook()
{
}

USpellBook::~USpellBook()
{
   // spellLookupTable->FindRow<FSpellbookLookupTable>()
}

void USpellBook::PostInitProperties()
{
   Super::PostInitProperties();
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
         if(spellObject) // valid spell?
         {
            // Give everyone level 1 of all the possible abilities hero can learn
            heroRef->GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(spellObject, 1));
            int spellID = spellObject->spellDefaults.id;
            // Also make a spell node with information and add it to node bank
            spellNodes.Add(spellID, SpellNode(i, TArray<SpellNode>(), TArray<SpellNode>(), spell));
            // if there's no prereqs and the required level is satisfied
            if(USpellManager::Get().GetSpellInfo(spellID)->preReqs.Num() == 0) //&& spellObject->GetReqLevel() > heroRef->GetLevel())
            {
               // Add pointer to learnable spells to act as starting nodes
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
            for(int preReqIndex : USpellManager::Get().GetSpellInfo(s.spellRef.GetDefaultObject()->spellDefaults.id)->preReqs) {
               if(spellNodes.Contains(preReqIndex)) // check if key exists in case spellID isn't coded
               {
                  s.prevSpellNodes.Add(spellNodes[preReqIndex]); // let node know of nodes that point to it
                  spellNodes[preReqIndex].nextSpellNodes.Add(s); // let nodes that point to node know they are pointing to this node
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
      if(heroRef->skillPoints <= 0) {
         cpcRef->GetHUDManager()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("Spellbook", "OutOfPoints", "Need more skill points!"));
         return false;
      }

      if(heroRef->GetUnitLevel() < spellObject->GetReqLevel(heroRef->GetAbilitySystemComponent())) {
         cpcRef->GetHUDManager()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("Spellbook", "LevelRequirementNotMet", "You are not high enough level to learn this spell"));
         return false;
      }

      for(int i : spellObject->GetPreReqs()) // if we have the prereqs go on
      {
         if(!GetLearnableSpells().Contains(i)) {
            cpcRef->GetHUDManager()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("Spellbook", "PrereqsNotMet", "You're missing some prerequisite skill(s)"));
            return false;
         }
      }

      FGameplayAbilitySpec* abilityInfo = heroRef->GetAbilitySystemComponent()->FindAbilitySpecFromClass(spellNodes[spellObject->spellDefaults.id].spellRef);
      if(!GetLearnedSpells().Contains(index)) // this spell has not been learned yet
      {
         cpcRef->GetHUDManager()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("Spellbook", "LearnNewSpell", "Learned a new spell"));
         learnedSpells.AddTail(spellNodes[spellObject->spellDefaults.id]);
         --heroRef->skillPoints;
         // Don't have to increment level since we start off abilities at level 1 anyways
         for(SpellNode s : spellNodes[spellObject->spellDefaults.id].nextSpellNodes) // if this spell unlocked new ones, then add those new ones to the learnable list
         {
            if(IsLearnable(s))
               learnableSpells.AddTail(s);
         }
         return true;
      } else // if we already learned this spell, level it up
      {
         if(abilityInfo->Level <
            USpellManager::Get().GetSpellInfo(availableSpells[index].GetDefaultObject()->spellDefaults.id)->maxLevel) // if we aren't at the max level already
         {
            ++abilityInfo->Level; // increment the level by 1
            cpcRef->GetHUDManager()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("Spellbook", "Upgrade", "Upgraded Ability!"));
            heroRef->GetAbilitySystemComponent()->MarkAbilitySpecDirty(*abilityInfo);
            --heroRef->skillPoints;
            return true;
         } else
            cpcRef->GetHUDManager()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("Spellbook", "UpgradeFailMax", "Ability Already at Max Level!"));
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
