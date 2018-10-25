// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "EffectStatusBar.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "WorldObjects/Unit.h"

#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "ResourceManager.h"

void UEffectStatusBar::NativeConstruct()
{
   Super::NativeConstruct();
   CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
}

void UEffectStatusBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
   Super::NativeTick(MyGeometry, InDeltaTime);
   UpdateStatusBar();
}

void UEffectStatusBar::UpdateStatusBar()
{
   // not a guaranteed way to reduce the amount of copies done because race conditions could occur where we have
   // one thread may remove an active gameplay effect and another thread may add one in before this check goes off meaning
   // we want to update the status bar but this check doesn't know that happened!  However, it's what I got for now

   // GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Black,
   // FString::FromInt(CPCRef->basePlayer->focusedUnit->GetAbilitySystemComponent()->GetActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(nameFilter)).Num()));
   if (effects.Num() != CPCRef->GetBasePlayer()->focusedUnit->GetAbilitySystemComponent()->GetActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(nameFilter)).Num()) {
      FGameplayTagContainer tagContainer; // container which is passed by reference to hold asset tags (which includes name and element tags)

      effects.Empty();
      ResetShownEffects();
      // We don't want cooldown effects which we unfortunately need since the CD mechanism we built relies on it
      effects = CPCRef->GetBasePlayer()->focusedUnit->GetAbilitySystemComponent()->GetActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(nameFilter));
      for (FActiveGameplayEffectHandle effect : effects) {
         effect.GetOwningAbilitySystemComponent()->GetActiveGameplayEffect(effect)->Spec.GetAllAssetTags(tagContainer);
         FGameplayTag nameTag = tagContainer.Filter(nameFilter).First();
#if UE_EDITOR
         checkf(nameTag != FGameplayTag::EmptyTag, TEXT("Name tag not found!!"))
             checkf(effectIconDatabase->effectIconMap.Contains(nameTag), TEXT("Tag name %s does not exist in list of nametags!!"), nameTag.GetTagName().GetPlainANSIString())
#endif
                 if (!currentlyStoredEffectsList.HasTagExact(
                         nameTag)) // if we don't have this effect already (checked since we can have multiple effects with same name since effect parts have to be split...)
         {
            UTexture2D*           effectIcon = effectIconDatabase->effectIconMap[nameTag];
            FGameplayTagContainer elemTags   = tagContainer.Filter(elemFilter);

            FLinearColor finalColor;
            for (FGameplayTag elemTag : elemTags) {
               finalColor = ResourceManager::elementalMap[elemTag].WithAlpha(0.5);
            }

            AddGameplayEffectIcon(effect, nameTag, effectIcon, finalColor);
         }
      }
   }
}
