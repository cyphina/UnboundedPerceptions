// Created 1/31/21 02:30 PM

#include "MyProject.h"
#include "RTSDamageNumberWidget.h"
#include "RTSDamageNumberContainer.h"
#include "TextBlock.h"
#include "DamageStructs.h"
#include "SpellDataLibrary.h"
#include "UWidgetHelperLibrary.h"

void URTSDamageNumberWidget::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   FWidgetAnimationDynamicEvent OnWidgetFinishedAnim;
   OnWidgetFinishedAnim.BindDynamic(this, &URTSDamageNumberWidget::OnFloatAnimationFinished);
   BindToAnimationFinished(floatAndDissapearAnimation, OnWidgetFinishedAnim);
}

void URTSDamageNumberWidget::SetDamageTextProps(const FUpDamage& damageInfo)
{
   if(damageText)
   {
      damageText->SetText(FText::AsNumber(damageInfo.damage));
      damageText->SetColorAndOpacity(FSlateColor(USpellDataLibrary::elementalMap[damageInfo.element]));
      PlayAnimation(floatAndDissapearAnimation);
   }
}

void URTSDamageNumberWidget::OnFloatAnimationFinished()
{
   if(URTSDamageNumberContainer* damageContainer = UUWidgetHelperLibrary::GetUserWidgetParent<URTSDamageNumberContainer>(this))
   {
      damageContainer->ReleaseDamageNumberWidget(this);
   }
}
