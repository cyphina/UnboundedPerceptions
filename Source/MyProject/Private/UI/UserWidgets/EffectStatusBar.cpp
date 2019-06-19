// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "EffectStatusBar.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "WorldObjects/Unit.h"

#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "UpResourceManager.h"

void UEffectStatusBar::NativeConstruct()
{
   Super::NativeConstruct();
   CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
}

void UEffectStatusBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
   Super::NativeTick(MyGeometry, InDeltaTime);

   //Won't work if we have some kind of effect that adds and removes an effect (like a burn + purge) 
   //if (effects.Num() != currentEffects.Num()) {}

   UpdateStatusBar();
}

void UEffectStatusBar::UpdateStatusBar()
{
   //GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Black, FString::FromInt(CPCRef->basePlayer->focusedUnit->GetAbilitySystemComponent()->GetActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(nameFilter)).Num()));
   if (IsValid(CPCRef->GetBasePlayer()->focusedUnit))
   {
      effects = CPCRef->GetBasePlayer()->focusedUnit->GetAbilitySystemComponent()->GetActiveEffects(
         FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(nameFilter));

      ResetShownEffects(); //Clear out the actionbar so we can redraw the updated version

      //Only shows the first 11 effects
      int numDisplaySlots = effects.Num() < maxNumDisplaySlots ? effects.Num() : 11;

      for (int i = 0; i < numDisplaySlots; ++i) {
         auto effect = effects[i];
         FGameplayTagContainer tagContainer; //Container which is passed by reference to hold asset tags (which includes name and element tags)
         effect.GetOwningAbilitySystemComponent()->GetActiveGameplayEffect(effect)->Spec.GetAllAssetTags(tagContainer); //Get all the tags of the effect, and filter it out by the effectName tag
         FGameplayTag nameTag = tagContainer.Filter(nameFilter).First();
#if UE_EDITOR
         checkf(nameTag != FGameplayTag::EmptyTag, TEXT("Name tag not found!!"));
         checkf(effectIconDatabase->effectIconMap.Contains(nameTag), TEXT("Tag name %s does not exist in list of nametags!!"), *nameTag.GetTagName().ToString());
#endif

         UTexture2D*           effectIcon = effectIconDatabase->effectIconMap[nameTag];
         FGameplayTagContainer elemTags = tagContainer.Filter(elemFilter);

         FLinearColor finalColor;
         for (FGameplayTag elemTag : elemTags) {
            finalColor = UpResourceManager::elementalMap[elemTag].WithAlpha(0.5);
         }

         ToggleGameplayEffectIcon(effect, nameTag, effectIcon, finalColor, i);
      }
   }
}

bool UEffectStatusBar::ActiveEffectHandleEqualityCheck(const FActiveGameplayEffectHandle& handle1, const FActiveGameplayEffectHandle& handle2) const
{
   return handle1 == handle2;
}
