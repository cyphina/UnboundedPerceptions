// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "SpellBook.h"
#include "WorldObjects/BaseHero.h"
#include "AbilitySystemComponent.h"
#include "MySpell.h"

bool USpellBook::isLearnable(SpellNode sNode)
{
   for (SpellNode s : sNode.prevSpellNodes) {
      if (!GetLearnedSpells().Contains(s.index)) return false;
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
   if (heroRef) {
      int i = 0;
      for (TSubclassOf<UMySpell> spell : availableSpells) {
         UMySpell* spellObject = spell.GetDefaultObject();
         if (spellObject) // valid spell?
         {
            // Give everyone level 1 of all the possible abilities hero can learn
            heroRef->GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(spellObject, 1));
            int spellID = spellObject->spellDefaults.id;
            // Also make a spell node with information and add it to node bank
            spellNodes.Add(spellID, SpellNode(i, TArray<SpellNode>(), TArray<SpellNode>(), spell));
            // if there's no prereqs and the required level is satisfied
            if (USpellManager::Get().GetSpellInfo(spellID)->preReqs.Num() == 0) //&& spellObject->GetReqLevel() > heroRef->GetLevel())
            {
               // add pointer to learnable spells to act as starting nodes
               learnableSpells.AddTail(spellNodes[spellID]);
               /*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt((&spellNodes[spellID])->index));
               GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::FromInt(learnableSpells.Last()->index));*/
            } else {
               unknownSpells.AddTail(spellNodes[spellID]);
               // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::FromInt(unknownSpells.Last()->index));
               /*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::FromInt((&spellNodes[spellID])->index));*/
            }
         }
         ++i;
      }

      /*GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::FromInt(unknownSpells.Num()));
      TArray<SpellNode> valueA;
      spellNodes.GenerateValueArray(valueA);
      for(SpellNode s : valueA)
      {
              GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(s.index));
      }*/

      // Setup connections
      for (SpellNode s : unknownSpells) {
         // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::FromInt(s.index));
         if (s.spellRef.GetDefaultObject()) // if valid spell class
         {
            // Loop through preReqs
            for (int preReqIndex : USpellManager::Get().GetSpellInfo(s.spellRef.GetDefaultObject()->spellDefaults.id)->preReqs) {
               if (spellNodes.Contains(preReqIndex)) // check if key exists in case spellID isn't coded
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
   UMySpell* spellObject = availableSpells[index].GetDefaultObject();
   // if we haven't learned this spell yet
   if (spellObject) {
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(spellObject->GetReqLevel(heroRef->GetAbilitySystemComponent())));
      if (heroRef->skillPoints > 0 && heroRef->GetLevel() >= spellObject->GetReqLevel(heroRef->GetAbilitySystemComponent())) {
         for (int i : spellObject->GetPreReqs()) // if we have the prereqs go on
         {
            if (!GetLearnableSpells().Contains(i)) {
               GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Prereqs not met"));
               return false;
            }
         }

         FGameplayAbilitySpec* abilityInfo = heroRef->GetAbilitySystemComponent()->FindAbilitySpecFromClass(spellNodes[spellObject->spellDefaults.id].spellRef);
         if (!GetLearnedSpells().Contains(index)) // this spell has not been learned yet
         {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Adding Ability"));
            learnedSpells.AddTail(spellNodes[spellObject->spellDefaults.id]);
            --heroRef->skillPoints;
            // Don't have to increment level since we start off abilities at level 1 anyways
            for (SpellNode s : spellNodes[spellObject->spellDefaults.id].nextSpellNodes) // if this spell unlocked new ones, then add those new ones to the learnable list
            {
               if (isLearnable(s)) learnableSpells.AddTail(s);
            }
            return true;
         } else // if we already learned this spell, level it up
         {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Upgrading Ability1"));
            if (abilityInfo->Level < USpellManager::Get().GetSpellInfo(availableSpells[index].GetDefaultObject()->spellDefaults.id)->maxLevel) // if we aren't at the max level already
            {
               ++abilityInfo->Level; // increment the level by 1
               heroRef->GetAbilitySystemComponent()->MarkAbilitySpecDirty(*abilityInfo);
               --heroRef->skillPoints;
               GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(abilityInfo->Level));
               return true;
            }
         }
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