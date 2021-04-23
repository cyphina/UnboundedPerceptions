#include "MyProject.h"
#include "EffectStatusBar.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "WorldObjects/Unit.h"

#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "EffectIconMap.h"
#include "EffectSlot.h"
#include "SpellDataLibrary.h"
#include "UpResourceManager.h"
#include "Engine/AssetManager.h"

void UEffectStatusBar::NativeOnInitialized()
{
   Super::NativeOnInitialized();
}

void UEffectStatusBar::NativeConstruct()
{
   Super::NativeConstruct();
   CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   GetWorld()->GetTimerManager().SetTimer(effectBarTimer, this, &UEffectStatusBar::UpdateStatusBar, 0.1f, true, 0.f);
   effects.Reserve(maxNumDisplaySlots);
}

void UEffectStatusBar::NativeDestruct()
{
   Super::NativeDestruct();
   GetWorld()->GetTimerManager().ClearTimer(effectBarTimer);
}

bool UEffectStatusBar::CheckEffectStillExistsAndAdjustIndex()
{
   if(AUnit* focusedUnit = CPCRef->GetBasePlayer()->GetFocusedUnit(); IsValid(focusedUnit))
   {
      const FActiveGameplayEffectHandle          effectToRemoveHandle = effects[GetSelectedSlotIndex()];
      const TArray<FActiveGameplayEffectHandle>& activeEffectHandles =
          focusedUnit->GetAbilitySystemComponent()->GetActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(USpellDataLibrary::GetEffectNameTag()));

      const int32 effectToRemoveHandleIndex = activeEffectHandles.Find(effectToRemoveHandle);
      if(effectToRemoveHandleIndex != INDEX_NONE)
      {
         SetSelectedSlotIndex(effectToRemoveHandleIndex);
         return true;
      }
   }
   return false;
}

FReply UEffectStatusBar::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   // Since the effects tracked by the status bar are cached based on a timer, there is a possibility that during the update period (set to .1 seconds by default) the effect expired so we can't actually  try to remove
   // one of the removeable effects since it doesn't actually exist.
   if(CheckEffectStillExistsAndAdjustIndex())
   {
      Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
   }
   return FReply::Handled();
}

void UEffectStatusBar::UpdateStatusBar()
{
   if(ABasePlayer* basePlayer = CPCRef->GetBasePlayer())
   {
      if(AUnit* focusedUnit = basePlayer->GetFocusedUnit(); IsValid(focusedUnit))
      {
         const FGameplayTagQuery tagQuery =
             FGameplayTagQuery::BuildQuery(FGameplayTagQueryExpression()
                                               .AllExprMatch()
                                               .AddExpr(FGameplayTagQueryExpression().AnyTagsMatch().AddTags(USpellDataLibrary::GetEffectNameTag()))
                                               .AddExpr(FGameplayTagQueryExpression().NoTagsMatch().AddTags(USpellDataLibrary::GetEffectPseudoStackTag())));
         FGameplayEffectQuery effectQuery;
         effectQuery.OwningTagQuery = tagQuery;
         effects                    = focusedUnit->GetAbilitySystemComponent()->GetActiveEffects(effectQuery);

         TArray<FActiveGameplayEffectHandle> pseudoStackableEffects =
             focusedUnit->GetAbilitySystemComponent()->GetActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(USpellDataLibrary::GetEffectPseudoStackTag()));

         TMap<TSubclassOf<UGameplayEffect>, FActiveGameplayEffectHandle> uniquePseudoStackableEffects;

         if(pseudoStackableEffects.Num() > 0)
         {
            for(const FActiveGameplayEffectHandle handle : pseudoStackableEffects)
            {
               const FActiveGameplayEffect* activeEffect = focusedUnit->GetAbilitySystemComponent()->GetActiveGameplayEffect(handle);

               if(activeEffect)
               {
                  if(!uniquePseudoStackableEffects.Contains(activeEffect->Spec.Def->GetClass()))
                  {
                     uniquePseudoStackableEffects.Add(activeEffect->Spec.Def->GetClass(), handle);
                  }
                  else
                  {
                     const FActiveGameplayEffectHandle currentShortestActiveEffectHandle = uniquePseudoStackableEffects[activeEffect->Spec.Def->GetClass()];
                     const float                       currentShorestEndTime =
                         focusedUnit->GetAbilitySystemComponent()->GetActiveGameplayEffect(currentShortestActiveEffectHandle)->GetEndTime();
                     if(activeEffect->GetEndTime() < currentShorestEndTime)
                     {
                        uniquePseudoStackableEffects[activeEffect->Spec.Def->GetClass()] = handle;
                     }
                  }
               }
            }
         }

         uniquePseudoStackableEffects.GenerateValueArray(pseudoStackableEffects);
         effects.Append(pseudoStackableEffects);

         ResetShownEffects();

         for(int i = 0; i < GetMaxNumberSlotsToDisplay(); ++i)
         {
            FActiveGameplayEffectHandle  effectHandle = effects[i];
            FGameplayTagContainer        effectAssetTags;
            const FActiveGameplayEffect* activeEffect = effectHandle.GetOwningAbilitySystemComponent()->GetActiveGameplayEffect(effectHandle);

            activeEffect->Spec.GetAllGrantedTags(effectAssetTags);
            activeEffect->Spec.GetAllAssetTags(effectAssetTags);

            FGameplayTag effectNameTag = effectAssetTags.Filter(USpellDataLibrary::GetEffectNameTag()).First();

            const FGameplayEffectSlotData* effectSlotData = effectIconDatabase->GetEffectIcon(effectNameTag);

            if(!ensure(effectSlotData))
            {
               UE_LOG(LogTemp, Error, TEXT("Effect tag does not have an icon in the data asset holding tag to icon maps"));
               break;
            }

            if(!ensure(effectNameTag != FGameplayTag::EmptyTag))
            {
               UE_LOG(LogTemp, Error, TEXT("Effect tag with empty name was added as a status effect"));
               break;
            }

            const FGameplayEffectSlotData* effectData = GetEffectSlotData(effectNameTag);

            effectSlots[i]->ActivateEffectSlot(effectHandle, *effectSlotData, GetEffectColor(effectAssetTags));
         }
      }
   }
}

FLinearColor UEffectStatusBar::GetEffectColor(const FGameplayTagContainer& effectAssetTags) const
{
   FLinearColor          finalColor = FColor::White;
   FGameplayTagContainer elemTags   = effectAssetTags.Filter(USpellDataLibrary::GetEffectElemTag());
   for(FGameplayTag elemTag : elemTags)
   {
      finalColor *= USpellDataLibrary::GetElementalColorMap()[elemTag].WithAlpha(0.5);
   }
   return finalColor;
}

int32 UEffectStatusBar::GetMaxNumberSlotsToDisplay()
{
   return effects.Num() < maxNumDisplaySlots ? effects.Num() : maxNumDisplaySlots;
}

bool UEffectStatusBar::ActiveEffectHandleEqualityCheck(const FActiveGameplayEffectHandle& handle1, const FActiveGameplayEffectHandle& handle2) const
{
   return handle1 == handle2;
}

const FGameplayEffectSlotData* UEffectStatusBar::GetEffectSlotData(const FGameplayTag& effectName) const
{
   return effectIconDatabase->GetEffectIcon(effectName);
}

void UEffectStatusBar::ResetShownEffects()
{
   for(UEffectSlot* effectSlot : effectSlots)
   {
      effectSlot->SetVisibility(ESlateVisibility::Hidden);
   }
}

int UEffectStatusBar::GetNumValidItems() const
{
   return effects.Num();
}
