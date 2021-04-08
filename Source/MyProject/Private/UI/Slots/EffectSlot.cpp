#include "MyProject.h"
#include "EffectSlot.h"
#include "SpellFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "EffectIconMap.h"
#include "GameplayEffectTypes.h"
#include "Image.h"
#include "MySpell.h"
#include "SpellDataLibrary.h"
#include "Globals/UpResourceManager.h"
#include "ToolTipWidget.h"

void UEffectSlot::SynchronizeProperties()
{
   // Can't drag these widgets
   Super::SynchronizeProperties();
   draggedActionWidgetClass = nullptr;
}

void UEffectSlot::ShowDesc(UToolTipWidget* tooltip)
{
   if(currentEffectSlotData)
   {
      const UAbilitySystemComponent* effectedUnitASC = currentActiveEffectHandle.GetOwningAbilitySystemComponent();
      if(effectedUnitASC)
      {
         if(UAbilitySystemComponent* casterASC =
                effectedUnitASC->GetActiveGameplayEffect(currentActiveEffectHandle)->Spec.GetEffectContext().GetOriginalInstigatorAbilitySystemComponent())
         {
            if(const FActiveGameplayEffect* activeEffect = effectedUnitASC->GetActiveGameplayEffect(currentActiveEffectHandle))
            {
               if(const UGameplayAbility* instigatingAbility = activeEffect->Spec.GetEffectContext().GetAbility())
               {
                  if(const UMySpell* instigatingSpell = Cast<UMySpell>(instigatingAbility))
                  {
                     tooltip->SetupTTBoxText(
                         FText::Format(NSLOCTEXT("EffectUI", "HoveredEffectSlotDesc", "{0} ({1})"), currentEffectSlotData->effectName, GetStackCount()),
                         USpellFunctionLibrary::ParseDesc(currentEffectSlotData->description, casterASC, instigatingSpell), GetEffectTogglableText(activeEffect->Spec),
                         FText::GetEmpty(), FText::GetEmpty());
                  }
               }
            }
         }
      }
   }
}

int32 UEffectSlot::GetStackCount() const
{
   FGameplayTagContainer          assetTags;
   const UAbilitySystemComponent* effectedUnitASC = currentActiveEffectHandle.GetOwningAbilitySystemComponent();

   if(effectedUnitASC)
   {
      if(const FActiveGameplayEffect* activeEffect = effectedUnitASC->GetActiveGameplayEffect(currentActiveEffectHandle))
      {
         activeEffect->Spec.GetAllAssetTags(assetTags);
         if(assetTags.HasTagExact(USpellDataLibrary::GetEffectPseudoStackTag().First()))
         {
            // TODO: See impact of this on my switch DYnamicAssetTags to DynamicGrantedTags
            return effectedUnitASC->GetActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(currentEffectSlotData->effectNameTag.GetSingleTagContainer()))
                .Num();
         }
         else
         {
            return activeEffect->Spec.StackCount;
         }
      }
   }
   return 0;
}

FText UEffectSlot::GetEffectTogglableText(const FGameplayEffectSpec& effectSpec)
{
   FGameplayTagContainer assetTags;
   effectSpec.GetAllAssetTags(assetTags);
   if(assetTags.HasTagExact(USpellDataLibrary::GetEffectRemoveableTag().First()))
   {
      return NSLOCTEXT("EffectUI", "ToggleableIndicator", "Can Toggle with Right Click");
   }
   return FText::GetEmpty();
}

void UEffectSlot::ActivateEffectSlot(FActiveGameplayEffectHandle activeEffectHandle, const FGameplayEffectSlotData& effectSlotData, const FLinearColor& color)
{
   actionImage->SetBrushFromSoftTexture(effectSlotData.effectIcon);
   currentActiveEffectHandle = activeEffectHandle;
   currentEffectSlotData     = &effectSlotData;
   SetVisibility(ESlateVisibility::Visible);
}

FReply UEffectSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
   {
      return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
   }
   return FReply::Handled();
}
