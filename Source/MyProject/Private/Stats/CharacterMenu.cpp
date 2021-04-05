#include "MyProject.h"
#include "CharacterMenu.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "WorldObjects/BaseHero.h"
#include "StatEnums.h"
#include "AttributePointSpenderWidget.h"
#include "Button.h"
#include "GameplayDelegateContext.h"
#include "PartyDelegateContext.h"
#include "StatgraphWidget.h"
#include "TextBlock.h"
#include "UIDelegateContext.h"
#include "UpStatComponent.h"
#include "VerticalBox.h"
#include "VerticalBoxSlot.h"
#include "WidgetCompilerLog.h"

void UCharacterMenu::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   CreateAttPointSpendingWidgets();

   Btn_Affinity->OnClicked.AddDynamic(this, &UCharacterMenu::ShowAffinities);
   Btn_Resist->OnClicked.AddDynamic(this, &UCharacterMenu::ShowDefenses);
   Btn_Vitals->OnClicked.AddDynamic(this, &UCharacterMenu::ShowVitals);
   Btn_Mechanics->OnClicked.AddDynamic(this, &UCharacterMenu::ShowMechanics);

   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UGameplayDelegateContext>()->OnMoneyGained().AddUObject(this, &UCharacterMenu::OnMoneyGained);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UGameplayDelegateContext>()->OnExpGained().AddUObject(this, &UCharacterMenu::OnEXPGained);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnHeroLevelUp().AddUObject(this, &UCharacterMenu::OnHeroLevelUp);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UUIDelegateContext>()->OnAttributePointAllocated().AddUObject(
   this, &UCharacterMenu::OnAttributePointAllocated);
}

bool UCharacterMenu::OnWidgetAddToViewport_Implementation()
{
   Super::OnWidgetAddToViewport_Implementation();
   baseHeroRef = CPC->GetBasePlayer()->GetSelectedHeroes()[0];
   if(baseHeroRef)
   {
      InitialHeroValueSetup(baseHeroRef);
      GetHeroWithInfoDisplayed()->GetStatComponent()->OnStatsUpdated().AddUObject(this, &UCharacterMenu::OnStatsUpdated);

      Widget_StatGraph->Startup();
      return true;
   }
   return false;
}

void UCharacterMenu::OnWidgetRemovedFromViewport_Implementation()
{
   Super::OnWidgetRemovedFromViewport_Implementation();
}

void UCharacterMenu::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
   Super::OnAnimationFinished_Implementation(Animation);
   if(IsAnimationPlayingForward(Animation))
   {
      Widget_StatGraph->ShowElementalOffensive();
   } else
   {
      Widget_StatGraph->Cleanup();
   }
}

void UCharacterMenu::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
   if(!AttributePointerWidgetClass)
   {
      CompileLog.Error(FText::Format(FText::FromString("{0} does not have a valid attribute pointer widget class set."), FText::FromString(GetName())));
   }
}

void UCharacterMenu::ShowAffinities()
{
   Widget_StatGraph->ShowElementalOffensive();
}

void UCharacterMenu::ShowDefenses()
{
   Widget_StatGraph->ShowElementalDefense();
}

void UCharacterMenu::ShowVitals()
{
   Widget_StatGraph->ShowVitals();
}

void UCharacterMenu::ShowMechanics()
{
   Widget_StatGraph->ShowMechanics();
}

void UCharacterMenu::InitialHeroValueSetup(ABaseHero* heroRef)
{
   Text_Name->SetText(heroRef->GetGameName());
   Text_Level->SetText(FText::AsNumber(heroRef->GetStatComponent()->GetUnitLevel()));
   Text_Money->SetText(FText::AsNumber(GetOwningPlayer<AUserInput>()->GetBasePlayer()->GetMoney()));
   Text_EXP->SetText(FText::AsNumber(heroRef->GetExpToLevel() - heroRef->GetCurrentExp()));
   Text_AttPoints->SetText(FText::AsNumber(heroRef->GetAttPoints()));
   SetupCharacterPortrait(heroRef);
}

void UCharacterMenu::OnMoneyGained(int moneyGained)
{
   const int money = GetOwningPlayer<AUserInput>()->GetBasePlayer()->GetMoney() + moneyGained;
   Text_Money->SetText(FText::AsNumber(money));
}

void UCharacterMenu::OnEXPGained(int expGained)
{
   // Ensure we're looking at a hero in the party (assuming those in party cannot gain EXP).
   if(GetOwningPlayer<AUserInput>()->GetBasePlayer()->GetHeroes().Contains(baseHeroRef))
   {
      const int expToLevel = baseHeroRef->GetExpToLevel() - baseHeroRef->GetCurrentExp();
      Text_EXP->SetText(FText::AsNumber(expToLevel));
   }
}

void UCharacterMenu::OnHeroLevelUp(ABaseHero* heroLevelingUp)
{
   if(heroLevelingUp == baseHeroRef)
   {
      Text_Level->SetText(FText::AsNumber(baseHeroRef->GetStatComponent()->GetUnitLevel()));
      Text_AttPoints->SetText(FText::AsNumber(baseHeroRef->GetAttPoints()));
      Text_EXP->SetText(FText::AsNumber(baseHeroRef->GetExpToLevel() - baseHeroRef->GetCurrentExp()));
   }
}

void UCharacterMenu::CreateAttPointSpendingWidgets()
{
   UVerticalBoxSlot* attBoxSlot = nullptr;
   for(int i = 0; i < static_cast<uint8>(EAttributes::Count); ++i)
   {
      UAttributePointSpenderWidget* attSpendWidget = UAttributePointSpenderWidget::MakePointSpenderWidget(this, AttributePointerWidgetClass, static_cast<EAttributes>(i));
      attBoxSlot                                   = Pan_Attributes->AddChildToVerticalBox(attSpendWidget);
      attBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
      attBoxSlot->SetPadding(FMargin(0, 0, 0, 4.f));
   }
   attBoxSlot->SetPadding(FMargin(0.f));
}

void UCharacterMenu::OnBaseStatsUpdated(const FGameplayAttribute& updatedBaseAttribute, float newValue, AUnit* updatedUnit)
{
   // TODO: Maybe play some kind of visual effect
}

void UCharacterMenu::OnStatsUpdated(const FGameplayAttribute& updatedAttribute, float newValue, AUnit* updatedUnit)
{
   if(updatedUnit == GetHeroWithInfoDisplayed())
   {
      for(UWidget* widget : Pan_Attributes->GetAllChildren())
      {
         UAttributePointSpenderWidget* attWidget = Cast<UAttributePointSpenderWidget>(widget);
         if(attWidget->GetAttCategory() == static_cast<EAttributes>(URTSAttributeSet::GetAtts().Find(updatedAttribute)))
         {
            // TODO: Maybe play some kind of visual effect
         }
      }
   }
}

void UCharacterMenu::OnAttributePointAllocated(ABaseHero* heroAllocating, EAttributes upgradedStat, bool bAllocated)
{
   Text_AttPoints->SetText(FText::AsNumber(baseHeroRef->GetAttPoints()));
}
