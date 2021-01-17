#include "MyProject.h"
#include "SpellBook.h"

#include "UserInput.h"

#include "WorldObjects/BaseHero.h"
#include "AIStuff/AIControllers/HeroAIController.h"
#include "HUDManager.h"
#include "UI/UserWidgets/RTSIngameWidget.h"

#include "AbilitySystemComponent.h"
#include "MySpell.h"
#include "SpellbookHUD.h"
#include "UpStatComponent.h"

#include "SpellDelegateStore.h"

void USpellBook::Init()
{
   if(heroRef)
   {
      cpcRef = Cast<AUserInput>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController());

      int index = 0;
      for(TSubclassOf<UMySpell> spell : availableSpells)
      {
         if(spell)
         {
            heroRef->GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(spell.GetDefaultObject(), 1));
            spellNodes.Add(spell, SpellNode(index, TArray<SpellNode*>(), TArray<SpellNode*>(), spell));
            CacheInitialLearnableAndUnknownSpells(spell);
         }
         ++index;
      }

      SetupInitialSpellNodeConnections();
      cpcRef->GetWidgetProvider()->GetIngameHUD()->GetSpellBookMenu()->OnSpellSlotSelected().AddUObject(this, &USpellBook::OnSpellSlotSelected);
   }
}

bool USpellBook::IsSpellLearnable(TSubclassOf<UMySpell> spellClass)
{
   for(auto preReq : spellClass.GetDefaultObject()->GetSpellDefaults().PreReqs)
   {
      SpellNode* s = &spellNodes[preReq];
      if(!learnedSpells.Contains(s))
         return false;
   }
   return true;
}

USpellBook* USpellBook::CreateSpellBook(ABaseHero* heroRef)
{
   USpellBook* spellbook = NewObject<USpellBook>(heroRef);
   spellbook->Init();
   spellbook->heroRef = heroRef;
   return spellbook;
}

TArray<int> USpellBook::GetLearnableSpellsIndices() const
{
   {
      TArray<int> spells;
      for(const SpellNode* sNode : learnableSpells)
      {
         spells.Emplace(sNode->index);
      }
      return spells;
   }
}

TArray<int> USpellBook::GetLearnedSpellIndices() const
{
   TArray<int> spells;
   for(const SpellNode* sNode : learnedSpells)
   {
      spells.Emplace(sNode->index);
   }
   return spells;
}

TArray<int> USpellBook::GetUnknownSpellIndices() const
{
   TArray<int> spells;
   for(const SpellNode* sNode : unknownSpells)
   {
      spells.Emplace(sNode->index);
   }
   return spells;
}

void USpellBook::CacheInitialLearnableAndUnknownSpells(TSubclassOf<UMySpell> spell)
{
   if(spell.GetDefaultObject()->GetSpellDefaults().PreReqs.Num() == 0)
   {
      learnableSpells.AddTail(&spellNodes[spell]);
   } else
   {
      unknownSpells.AddTail(&spellNodes[spell]);
   }
}

void USpellBook::SetupInitialSpellNodeConnections()
{
   for(SpellNode* s : unknownSpells)
   {
      for(const TSubclassOf<UMySpell> preReqSpell : s->spellRef.GetDefaultObject()->GetSpellDefaults().PreReqs)
      {
         if(spellNodes.Contains(preReqSpell))
         {
            SpellNode* sNode = &spellNodes[preReqSpell];
            s->prereqSpellNodes.Add(sNode);
            spellNodes[preReqSpell].unlockedSpellNodes.Add(s);
         }
      }
   }
}

TSubclassOf<UMySpell> USpellBook::GetSpellFromIndex(int slotIndex) const
{
   if(slotIndex < availableSpells.Num())
   {
      return availableSpells[slotIndex];
   }
   return nullptr;
}

void USpellBook::OnRespec()
{
   // TODO: Implement this
}

bool USpellBook::LearnNewSpell(TSubclassOf<UMySpell> spellToLearn)
{
   URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "LearnNewSpell", "Learned a new spell"));
   learnedSpells.AddTail(&spellNodes[spellToLearn]);
   learnableSpells.RemoveNode(&spellNodes[spellToLearn]);
   SpellHUDEvents::OnSpellLearnedEvent.Broadcast(spellToLearn);

   for(SpellNode* s : spellNodes[spellToLearn].unlockedSpellNodes)
   {
      if(IsSpellLearnable(s->spellRef))
      {
         unknownSpells.RemoveNode(s);
         learnableSpells.AddTail(s);
      }
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
      SpellHUDEvents::OnSpellUpgradedEvent.Broadcast(spellObject->GetClass());
      return true;
   }
   URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "UpgradeFailMax", "Ability Already at Max Level!"));
   return false;
}

void USpellBook::OnSpellSlotSelected(int index)
{
   check(index >= 0 && index < availableSpells.Num());
   UMySpell* spellObject = availableSpells[index].GetDefaultObject();

   if(spellObject)
   {
      if(!CheckLevel(spellObject))
      {
         return;
      }
      if(!CheckPoints())
      {
         return;
      }
      if(!CheckPrereqs(spellObject))
      {
         return;
      }
      FGameplayAbilitySpec* abilityInfo = heroRef->GetAbilitySystemComponent()->FindAbilitySpecFromClass(availableSpells[index]);

      if(!GetLearnedSpellIndices().Contains(index))
      {
         LearnNewSpell(availableSpells[index]);
      }
      TryUpgradeSpellLevel(spellObject, abilityInfo);
   }
}

bool USpellBook::CheckLevel(UMySpell* spellObject) const
{
   if(heroRef->GetStatComponent()->GetUnitLevel() < spellObject->GetReqLevel(heroRef->GetAbilitySystemComponent()))
   {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(),
                                              NSLOCTEXT("Spellbook", "LevelRequirementNotMet", "You are not high enough level to learn this spell"));
      return false;
   }
   return true;
}

bool USpellBook::CheckPoints() const
{
   if(heroRef->GetSkillPoints() <= 0)
   {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "OutOfPoints", "Need more skill points!"));
      return false;
   }
   return true;
}

bool USpellBook::CheckPrereqs(UMySpell* spellObject)
{
   for(TSubclassOf<UMySpell> spellPreReqClass : spellObject->GetPreReqs())
   {
      SpellNode* preReqSpellNode = &spellNodes[spellPreReqClass];

      if(!learnableSpells.Contains(preReqSpellNode))
      {
         URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "PrereqsNotMet", "You're missing some prerequisite skill(s)"));
         return false;
      }
   }
   return true;
}

bool USpellBook::CheckIfSpellMaxLevel(FGameplayAbilitySpec* abilityInfo, UMySpell* spellObject) const
{
   return abilityInfo->Level >
          spellObject->GetSpellDefaults().MaxLevel;
}


