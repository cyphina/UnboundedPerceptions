#include "MyProject.h"
#include "AttributePointSpenderWidget.h"


#include "BaseHero.h"
#include "CharacterMenu.h"
#include "UWidgetHelperLibrary.h"
#include "TextBlock.h"
#include "UIDelegateContext.h"

UAttributePointSpenderWidget* UAttributePointSpenderWidget::MakePointSpenderWidget(UUserWidget* widgetParent, TSubclassOf<UAttributePointSpenderWidget> widgetClass, EAttributes attCategory)
{
   UAttributePointSpenderWidget* attSpendWidget = CreateWidget<UAttributePointSpenderWidget>(widgetParent, widgetClass);
   attSpendWidget->attCategory = attCategory;
   return attSpendWidget;
}

void UAttributePointSpenderWidget::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   Text_Category->SetText(UEnum::GetDisplayValueAsText(attCategory));
}

void UAttributePointSpenderWidget::OnAttributeIncreased()
{
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UUIDelegateContext>()->OnAttributePointAllocated().Broadcast(attCategory, true);
}

void UAttributePointSpenderWidget::OnAttributeDecreased()
{
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UUIDelegateContext>()->OnAttributePointAllocated().Broadcast(attCategory, false);
}

