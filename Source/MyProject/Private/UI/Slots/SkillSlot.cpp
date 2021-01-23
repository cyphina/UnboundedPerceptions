#include "MyProject.h"
#include "SkillSlot.h"
#include "ActionSlot.h"
#include "UserInput.h"
#include "../HUDManager.h"
#include "SpellSystem/MySpell.h"

#include "WorldObjects/Ally.h"
#include "AIStuff/AIControllers/AllyAIController.h"

#include "AbilitySystemComponent.h"
#include "BasePlayer.h"
#include "ManualSpellComponent.h"
#include "RTSAbilitySystemComponent.h"
#include "ToolTipWidget.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"

class UUIDelegateContext;
UMaterialInterface* USkillSlot::matInstance = nullptr;

USkillSlot::USkillSlot(const FObjectInitializer& o) : UActionSlot(o)
{
   // static ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Game/RTS_Tutorial/HUDs/ActionUI/StandardLinear"));
   // checkf(curve.Object, TEXT("Curve for ability timelines not found!"))

   const ConstructorHelpers::FObjectFinder<UMaterialInterface> loadedBaseMaterial(TEXT("/Game/RTS_Tutorial/Materials/UIMats/SkillMats/RadialMatSkill_Instance"));
   checkf(loadedBaseMaterial.Object, TEXT("Material interface for skillslots not found!"));
   matInstance = loadedBaseMaterial.Object;
}

void USkillSlot::NativeConstruct()
{
   Super::NativeConstruct();
   imageDMatInst = UMaterialInstanceDynamic::Create(matInstance, this);
   cdDMatInst    = UMaterialInstanceDynamic::Create(matInstance, this);
   actionImage->SetBrushFromMaterial(imageDMatInst);
   Image_CD->SetBrushFromMaterial(cdDMatInst);
}

void USkillSlot::UpdateSkillSlot(TSubclassOf<UMySpell> spellClass)
{
   if(IsValid(spellClass)) {
      UMySpell* spellObject = spellClass.GetDefaultObject();

      SetSlotImage(spellObject->GetSpellDefaults().image);

      if(URTSAbilitySystemComponent* ownerAbilitySystemComp = GetOwningAbilityComponent()) {
         const bool bIsSpellOffCooldown = spellObject->GetCooldownTimeRemaining(ownerAbilitySystemComp->AbilityActorInfo.Get()) > SMALL_NUMBER;

         if(bIsSpellOffCooldown) {
            ShowCooldown();
         } else {
            OnCDFinished();
         }
      }
   } else {
      ResetSkillSlot();
   }
}

void USkillSlot::UpdateCD()
{
   const auto  abilityComponent = GetOwningAbilityComponent();
   const float cdTimeRemaining =
       abilityComponent->GetAbilities()[slotIndex].GetDefaultObject()->GetCooldownTimeRemaining(GetOwningAbilityComponent()->AbilityActorInfo.Get());
   const float cdDuration = abilityComponent->GetAbilities()[slotIndex].GetDefaultObject()->GetCDDuration(GetOwningAbilityComponent());

   if(LIKELY(cdTimeRemaining > 0)) {
      cdDMatInst->SetScalarParameterValue("Percent", cdTimeRemaining / cdDuration);
      Text_CDTime->SetText(FText::AsNumber(static_cast<int>(cdTimeRemaining)));
   } else {
      OnCDFinished();
   }
}

void USkillSlot::OnCDFinished()
{
   GetWorld()->GetTimerManager().ClearTimer(cooldownProgressTimer);
   Image_CD->SetVisibility(ESlateVisibility::Hidden);
   Text_CDTime->SetVisibility(ESlateVisibility::Hidden);
}

void USkillSlot::ShowCooldown()
{
   GetWorld()->GetTimerManager().SetTimer(cooldownProgressTimer, this, &USkillSlot::UpdateCD, .1f, true, 0);
   ShowCDVisuals();
}

void USkillSlot::ShowCDVisuals() const
{
   Image_CD->SetVisibility(ESlateVisibility::HitTestInvisible);
   Text_CDTime->SetVisibility(ESlateVisibility::HitTestInvisible);
}



URTSAbilitySystemComponent* USkillSlot::GetOwningAbilityComponent() const
{
   if(const AUnit* focusedUnit = GetOwningPlayer<AUserInput>()->GetBasePlayer()->GetFocusedUnit()) {
      if(URTSAbilitySystemComponent* abilityComp = focusedUnit->FindComponentByClass<URTSAbilitySystemComponent>()) { return abilityComp; }
   }
   return nullptr;
}

void USkillSlot::SetSlotImage(UTexture2D* image)
{
   if(IsValid(image)) {
      imageDMatInst->SetTextureParameterValue("RadialTexture", image);
      SetImageFromMaterial(imageDMatInst);
      cdDMatInst->SetTextureParameterValue("RadialTexture", image); // update the cooldown image
      Image_CD->SetBrushFromMaterial(cdDMatInst);
   } else {
      imageDMatInst->SetTextureParameterValue("RadialTexture", defaultSlotTexture);
      SetImageFromMaterial(imageDMatInst);
   }
}

void USkillSlot::ResetSkillSlot()
{
   if(auto ownerAbilityComp = GetOwningAbilityComponent()) {
      SetSlotImage(nullptr);
      OnCDFinished();
   }
}

void USkillSlot::ShowDesc(UToolTipWidget* tooltip)
{
   if(const auto ownerAbilityComp = GetOwningAbilityComponent()) {
      if(const TSubclassOf<UMySpell> spellClass = ownerAbilityComp->GetSpellAtSlot(slotIndex)) {
         UMySpell*     spellAtSlot       = spellClass.GetDefaultObject();
         const FString relevantSpellInfo = "Costs " + FString::FromInt(spellAtSlot->GetCost(ownerAbilityComp)) + " mana\r\n" +
                                           FString::FromInt(spellAtSlot->GetCDDuration(ownerAbilityComp)) + " second CD \r\n" +
                                           FString::FromInt(spellAtSlot->GetRange(ownerAbilityComp)) + " range";
         tooltip->SetupTTBoxText(spellAtSlot->GetSpellName(), spellAtSlot->GetDescription(), spellAtSlot->GetElem(), FText::FromString(relevantSpellInfo), FText::GetEmpty());
      }
   }
}

void USkillSlot::OnBtnClick()
{
   // One place the UI drives gameplay logic since the extra rebindings to invert the dependency would be suboptimal
   if(const AUnit* focusedUnit = GetOwningPlayer<AUserInput>()->GetBasePlayer()->GetFocusedUnit()) {
      if(UManualSpellComponent* manualSpellComp = focusedUnit->FindComponentByClass<UManualSpellComponent>()) { manualSpellComp->PressedCastSpell(slotIndex); }
   }
}
