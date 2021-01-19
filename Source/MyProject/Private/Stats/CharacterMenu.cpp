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
#include "UpStatComponent.h"
#include "VerticalBox.h"
#include "VerticalBoxSlot.h"

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
}

bool UCharacterMenu::OnWidgetAddToViewport_Implementation()
{
   Super::OnWidgetAddToViewport_Implementation();
   baseHeroRef = CPC->GetBasePlayer()->selectedHeroes[0];
   return baseHeroRef ? true : false;
}

void UCharacterMenu::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
   Super::OnAnimationFinished_Implementation(Animation);
   if(IsAnimationPlayingForward(Animation))
   {
      Widget_StatGraph->ShowElementalOffensive();
   }
   else
   {
      Widget_StatGraph->Cleanup();
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
   }
}

void UCharacterMenu::CreateAttPointSpendingWidgets()
{
   for(EAttributes attribute : TEnumRange<EAttributes>())
   {
      UAttributePointSpenderWidget* attSpendWidget = UAttributePointSpenderWidget::MakePointSpenderWidget(this, AttributePointerWidgetClass, attribute);
      UVerticalBoxSlot*             attBoxSlot     = Pan_Attributes->AddChildToVerticalBox(attSpendWidget);
      attBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
   }
}
