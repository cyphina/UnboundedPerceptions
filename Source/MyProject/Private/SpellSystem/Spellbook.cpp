#include "MyProject.h"
#include "SpellBook.h"

#include "UserInput.h"

#include "WorldObjects/BaseHero.h"
#include "AIStuff/AIControllers/HeroAIController.h"
#include "HUDManager.h"
#include "UI/UserWidgets/RTSIngameWidget.h"

#include "AbilitySystemComponent.h"
#include "MySpell.h"
<<<<<<< HEAD
#include "UpStatComponent.h"
=======
#include "SpellbookHUD.h"
#include "UpStatComponent.h"

#include "SpellDelegateStore.h"
>>>>>>> componentrefactor

void USpellBook::Init()
{
<<<<<<< HEAD
   for(SpellNode s : sNode.prereqSpellNodes) {
      if(!GetLearnedSpells().Contains(s.index)) return false;
=======
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
      GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
         cpcRef->GetWidgetProvider()->GetIngameHUD()->GetSpellBookMenu()->OnSlotSelected().AddUObject(this, &USpellBook::OnSpellSlotSelected);
      });
>>>>>>> componentrefactor
   }
}

bool USpellBook::IsSpellLearnable(TSubclassOf<UMySpell> spellClass)
{
   for(auto preReq : spellClass.GetDefaultObject()->GetSpellDefaults().PreReqs)
   {
      if(!learnedSpells.Contains(preReq)) return false;
   }
   return true;
}

<<<<<<< HEAD
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
=======
USpellBook* USpellBook::CreateSpellBook(ABaseHero* heroRef, TSubclassOf<USpellBook> spellBookClass)
{
   if(spellBookClass)
   {
      USpellBook* spellbook = NewObject<USpellBook>(heroRef, spellBookClass);
      spellbook->heroRef    = heroRef;
      spellbook->Init();
      return spellbook;
   }
   return nullptr;
}

TArray<int> USpellBook::GetLearnableSpellsIndices() const
{
   TArray<int> spellIndices;
   for(TSubclassOf<UMySpell> spellClass : learnableSpells)
   {
      spellIndices.Add(spellNodes[spellClass].index);
>>>>>>> componentrefactor
   }
   return spellIndices;
}

TArray<int> USpellBook::GetLearnedSpellIndices() const
{
   TArray<int> spellIndices;
   for(TSubclassOf<UMySpell> spellClass : learnedSpells)
   {
      spellIndices.Add(spellNodes[spellClass].index);
   }
   return spellIndices;
}

TArray<int> USpellBook::GetUnknownSpellIndices() const
{
   TArray<int> spellIndices;
   for(TSubclassOf<UMySpell> spellClass : unknownSpells)
   {
      spellIndices.Add(spellNodes[spellClass].index);
   }
   return spellIndices;
}

<<<<<<< HEAD
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
=======
void USpellBook::CacheInitialLearnableAndUnknownSpells(TSubclassOf<UMySpell> spell)
{
   if(spell.GetDefaultObject()->GetSpellDefaults().PreReqs.Num() == 0)
   {
      learnableSpells.AddTail(spell);
   }
   else
   {
      unknownSpells.AddTail(spell);
   }
}
>>>>>>> componentrefactor

void USpellBook::SetupInitialSpellNodeConnections()
{
   for(TSubclassOf<UMySpell> spellClass : unknownSpells)
   {
      SpellNode* s = &spellNodes[spellClass];
      for(const TSubclassOf<UMySpell> preReqSpell : s->spellRef.GetDefaultObject()->GetSpellDefaults().PreReqs)
      {
         if(spellNodes.Contains(preReqSpell))
         {
<<<<<<< HEAD
            // Loop through preReqs
            for(int preReqIndex : USpellDataManager::GetData().GetSpellInfo(s.spellRef.GetDefaultObject()->spellDefaults.id)->preReqs) {
               if(spellNodes.Contains(preReqIndex)) // check if key exists in case spellID isn't coded
               {
                  s.prereqSpellNodes.Add(spellNodes[preReqIndex]);   // let node know of nodes that point to it
                  spellNodes[preReqIndex].unlockedSpellNodes.Add(s); // let nodes that point to node know they are pointing to this node
               }
            }
=======
            SpellNode* sNode = &spellNodes[preReqSpell];
            s->prereqSpellNodes.Add(sNode);
            spellNodes[preReqSpell].unlockedSpellNodes.Add(s);
>>>>>>> componentrefactor
         }
      }
   }
}

TSubclassOf<UMySpell> USpellBook::GetSpellFromIndex(int slotIndex) const
{
<<<<<<< HEAD
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
=======
   if(slotIndex < availableSpells.Num())
   {
      return availableSpells[slotIndex];
   }
   return nullptr;
>>>>>>> componentrefactor
}

void USpellBook::OnRespec()
{
<<<<<<< HEAD
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
=======
   // TODO: Implement this
}

bool USpellBook::LearnNewSpell(TSubclassOf<UMySpell> spellToLearn)
{
   if(heroRef)
   {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "LearnNewSpell", "Learned a new spell"));
      learnedSpells.AddTail(spellToLearn);
      learnableSpells.RemoveNode(spellToLearn);
      SpellGameContext::OnSpellLearnedEvent.Broadcast(*heroRef, spellToLearn);

      for(SpellNode* s : spellNodes[spellToLearn].unlockedSpellNodes)
      {
         if(IsSpellLearnable(s->spellRef))
         {
            unknownSpells.RemoveNode(s->spellRef);
            learnableSpells.AddTail(spellToLearn);
         }
      }
      return true;
   }
   return false;
}

bool USpellBook::TryUpgradeSpellLevel(UMySpell* spellObject, FGameplayAbilitySpec* abilityInfo)
{
   if(heroRef)
   {
      if(!CheckIfSpellMaxLevel(abilityInfo, spellObject))
      {
         ++abilityInfo->Level;
         URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "Upgrade", "Upgraded Ability!"));
         heroRef->GetAbilitySystemComponent()->MarkAbilitySpecDirty(*abilityInfo);
         SpellGameContext::OnSpellUpgradedEvent.Broadcast(*heroRef, spellObject->GetClass());
         return true;
      }
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "UpgradeFailMax", "Ability Already at Max Level!"));
      return false;
   }
   return false;
}

void USpellBook::OnSpellSlotSelected(int index)
{
   if(cpcRef->GetWidgetProvider()->GetIngameHUD()->GetSpellBookMenu()->GetHeroRef()->GetSpellBook() == this)
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
         else
         {
            TryUpgradeSpellLevel(spellObject, abilityInfo);
         }
>>>>>>> componentrefactor
      }
   }
}

bool USpellBook::CheckLevel(UMySpell* spellObject) const
{
<<<<<<< HEAD
   if(heroRef->GetStatComponent()->GetUnitLevel() < spellObject->GetReqLevel(heroRef->GetAbilitySystemComponent())) {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(),
                                              NSLOCTEXT("Spellbook", "LevelRequirementNotMet", "You are not high enough level to learn this spell"));
=======
   if(heroRef->GetStatComponent()->GetUnitLevel() < spellObject->GetReqLevel(heroRef->GetAbilitySystemComponent()))
   {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "LevelRequirementNotMet", "You are not high enough level to learn this spell"));
>>>>>>> componentrefactor
      return false;
   }
   return true;
}

bool USpellBook::CheckPoints() const
{
<<<<<<< HEAD
   if(heroRef->GetSkillPoints() <= 0) {
=======
   if(heroRef->GetSkillPoints() <= 0)
   {
>>>>>>> componentrefactor
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "OutOfPoints", "Need more skill points!"));
      return false;
   }
   return true;
}

<<<<<<< HEAD
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
=======
bool USpellBook::CheckPrereqs(UMySpell* spellObject)
>>>>>>> componentrefactor
{
   for(TSubclassOf<UMySpell> spellPreReqClass : spellObject->GetPreReqs())
   {
      if(!learnedSpells.Contains(spellPreReqClass))
      {
         URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Spellbook", "PrereqsNotMet", "You're missing some prerequisite skill(s)"));
         return false;
      }
   }
   return true;
}

bool USpellBook::CheckIfSpellMaxLevel(FGameplayAbilitySpec* abilityInfo, UMySpell* spellObject) const
{
   return abilityInfo->Level >= spellObject->GetSpellDefaults().MaxLevel;
}
