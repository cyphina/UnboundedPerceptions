#include "MyProject.h"
#include "AttributePointSpenderWidget.h"
#include "BaseHero.h"
#include "Button.h"
#include "CharacterMenu.h"
#include "UpWidgetHelperLibrary.h"
#include "TextBlock.h"
#include "UIDelegateContext.h"
#include "UpStatComponent.h"

UAttributePointSpenderWidget* UAttributePointSpenderWidget::MakePointSpenderWidget(UUserWidget* widgetParent, TSubclassOf<UAttributePointSpenderWidget> widgetClass,
                                                                                   EAttributes attCategory)
{
   UAttributePointSpenderWidget* attSpendWidget = CreateWidget<UAttributePointSpenderWidget>(widgetParent, widgetClass);
   attSpendWidget->attCategory                  = attCategory;
   attSpendWidget->Text_Category->SetText(UEnum::GetDisplayValueAsText(attCategory));
   return attSpendWidget;
}

void UAttributePointSpenderWidget::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   parentWidget = UUpWidgetHelperLibrary::GetUserWidgetParent<UCharacterMenu>(this);
   Text_Value->TextDelegate.BindUFunction(this, "GetAttributeValues");
   Btn_Increment->OnClicked.AddDynamic(this, &UAttributePointSpenderWidget::OnAttributeIncreased);
   Btn_Decrement->OnClicked.AddDynamic(this, &UAttributePointSpenderWidget::OnAttributeDecreased);
}

FText UAttributePointSpenderWidget::GetAttributeValues()
{
   ABaseHero* heroBeingDisplayed = parentWidget->GetHeroWithInfoDisplayed();
   return FText::Format(FText::FromString("{0}/{1}"), heroBeingDisplayed->GetStatComponent()->GetAttributeBaseValue(attCategory),
                        heroBeingDisplayed->GetStatComponent()->GetAttributeAdjValue(attCategory));
}

void UAttributePointSpenderWidget::OnAttributeIncreased()
{
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UUIDelegateContext>()->OnAttributePointAllocated().Broadcast(parentWidget->GetHeroWithInfoDisplayed(),
                                                                                                                              attCategory, true);
}

void UAttributePointSpenderWidget::OnAttributeDecreased()
{
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UUIDelegateContext>()->OnAttributePointAllocated().Broadcast(parentWidget->GetHeroWithInfoDisplayed(),
                                                                                                                              attCategory, false);
}
