// Created 1/31/21 02:30 PM

#include "MyProject.h"
#include "RTSDamageNumberWidget.h"
#include "RTSDamageNumberContainer.h"
#include "TextBlock.h"
#include "DamageStructs.h"
#include "SpellDataLibrary.h"
#include "UpWidgetHelperLibrary.h"

void URTSDamageNumberWidget::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   FWidgetAnimationDynamicEvent OnWidgetFinishedAnim;
   OnWidgetFinishedAnim.BindDynamic(this, &URTSDamageNumberWidget::OnFloatAnimationFinished);
   BindToAnimationFinished(floatAndDissapearAnimation, OnWidgetFinishedAnim);
}

void URTSDamageNumberWidget::SetDamageTextProps(const FUpDamage& damageInfo, bool bIsHealing)
{
   FString damageString = FString::FromInt(damageInfo.damage);
   if(damageText)
   {
      if(damageInfo.DidMiss())
      {
         damageText->SetText(NSLOCTEXT("DamageNumbers", "Dodge", "Dodged!"));
      }
      else
      {
         damageText->SetColorAndOpacity(FSlateColor(USpellDataLibrary::elementalMap[damageInfo.element]));

         if(damageInfo.crit)
         {
            damageString += "!";
         }

         if(bIsHealing)
         {
            damageString = "+" + damageString;
         }

         damageText->SetRenderScale(FVector2D(1.5f));

         damageText->SetText(FText::Format(NSLOCTEXT("DamageNumbers", "DamageFormat", "{0}"), FText::FromString(damageString)));

         damageText->SetColorAndOpacity(FSlateColor(USpellDataLibrary::elementalMap[damageInfo.element]));
      }
      PlayAnimation(floatAndDissapearAnimation);
   }
}

void URTSDamageNumberWidget::OnFloatAnimationFinished()
{
   if(URTSDamageNumberContainer* damageContainer = UUpWidgetHelperLibrary::GetUserWidgetParent<URTSDamageNumberContainer>(this))
   {
      damageContainer->ReleaseDamageNumberWidget(this);
   }
}
