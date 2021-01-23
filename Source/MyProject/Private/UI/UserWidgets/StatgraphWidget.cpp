// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StatgraphWidget.h"


#include "AbilitySystemComponent.h"
#include "BaseCharacter.h"
#include "BaseHero.h"
#include "BasePlayer.h"
#include "BluEye.h"
#include "HUDTypes.h"
#include "WidgetToggler.h"
#include "Unit.h"
#include "UpStatComponent.h"
#include "UserInput.h"

void UStatgraphWidget::NativeOnInitialized()
{
   Super::NativeConstruct();
   cpcRef = Cast<AUserInput>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController());
}

void UStatgraphWidget::Startup()
{
   if(auto focusedHero = Cast<ABaseHero>(CheckIfFocusedUnitHero()); focusedHero) {
      UMyAttributeSet* attSet = const_cast<UMyAttributeSet*>(focusedHero->GetAbilitySystemComponent()->GetSet<UMyAttributeSet>());
      statListeners           = MakeTuple(attSet->statUpdatedEvent.AddUObject(this, &UStatgraphWidget::UpdateStat),
                                attSet->baseStatUpdatedEvent.AddUObject(this, &UStatgraphWidget::UpdateBaseStat));
      StartDisplay(width, height);
   }
}

void UStatgraphWidget::Cleanup()
{
   if(auto focusedHero = Cast<ABaseHero>(CheckIfFocusedUnitHero()); focusedHero) {
      if(ensure(statListeners.Key.IsValid() && statListeners.Value.IsValid())) {
         UMyAttributeSet* attSet = const_cast<UMyAttributeSet*>(focusedHero->GetAbilitySystemComponent()->GetSet<UMyAttributeSet>());
         attSet->statUpdatedEvent.Remove(statListeners.Key);
         attSet->baseStatUpdatedEvent.Remove(statListeners.Value);
         browser->CloseBrowser();
      }
   }
}

void UStatgraphWidget::ShowElementalOffensive()
{
   if(const auto focusedUnit = CheckIfFocusedUnitHero(); focusedUnit) {
      FString      heroInfoString;
      auto         writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&heroInfoString);
      const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUnitScalingStats"), true);

      writer->WriteArrayStart();
      for(auto stat : TEnumRange<EUnitScalingStats>()) {
         if(const auto& statName = eBonus->GetNameStringByIndex(static_cast<uint8>(stat)); statName.EndsWith("_Aff")) {
            writer->WriteObjectStart();
            writer->WriteValue("statName", statName);
            writer->WriteIdentifierPrefix("values");
            writer->WriteArrayStart();
            writer->WriteValue(focusedUnit->GetStatComponent()->GetSkillBaseValue(stat));
            writer->WriteValue(focusedUnit->GetStatComponent()->GetSkillAdjValue(stat));
            writer->WriteArrayEnd();
            writer->WriteObjectEnd();
         }
      }
      writer->WriteArrayEnd();
      writer->Close();
      UpdateInformation("updateStatInfo", heroInfoString);
   } else
      UE_LOG(LogTemp, Error, TEXT("Somehow stat browser widget was told to read data from a null or non-hero"));
}

void UStatgraphWidget::ShowElementalDefense()
{
   if(const auto focusedUnit = CheckIfFocusedUnitHero(); focusedUnit) {
      FString      heroInfoString;
      auto         writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&heroInfoString);
      const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUnitScalingStats"), true);

      writer->WriteArrayStart();

      for(auto stat : TEnumRange<EUnitScalingStats>()) {
         if(const auto& statName = eBonus->GetNameStringByIndex(static_cast<uint8>(stat)); statName.EndsWith("_Resist")) {
            writer->WriteObjectStart();
            writer->WriteValue("statName", statName);
            writer->WriteIdentifierPrefix("values");
            writer->WriteArrayStart();
            writer->WriteValue(focusedUnit->GetStatComponent()->GetSkillBaseValue(stat));
            writer->WriteValue(focusedUnit->GetStatComponent()->GetSkillAdjValue(stat));
            writer->WriteArrayEnd();
            writer->WriteObjectEnd();
         }
      }
      writer->WriteArrayEnd();

      writer->Close();
      UpdateInformation("updateStatInfo", heroInfoString);

   } else
      UE_LOG(LogTemp, Error, TEXT("Somehow stat browser widget was told to read data from a null or non-hero"));
}

void UStatgraphWidget::ShowMechanics()
{
   if(const auto focusedUnit = CheckIfFocusedUnitHero(); focusedUnit) {
      FString      heroInfoString;
      auto         writer  = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&heroInfoString);
      const UEnum* eBonus  = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMechanics"), true);
      const UEnum* eBonus2 = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUnitScalingStats"), true);

      writer->WriteArrayStart();
      for(int i = 0; i < static_cast<int>(EMechanics::Count); ++i) {
         const EMechanics mech = static_cast<EMechanics>(i);
         writer->WriteObjectStart();
         writer->WriteValue("statName", eBonus->GetNameStringByIndex(i));
         writer->WriteIdentifierPrefix("values");
         writer->WriteArrayStart();
         writer->WriteValue(focusedUnit->GetStatComponent()->GetMechanicBaseValue(mech));
         writer->WriteValue(focusedUnit->GetStatComponent()->GetMechanicAdjValue(mech));
         writer->WriteArrayEnd();
         writer->WriteObjectEnd();
      }

      for(auto stat : TEnumRange<EUnitScalingStats>()) {
         if(const auto& statName = eBonus2->GetNameStringByIndex(static_cast<uint8>(stat)); !statName.EndsWith("_Resist") && !statName.EndsWith("_Aff")) {
            writer->WriteObjectStart();
            writer->WriteValue("statName", eBonus2->GetNameStringByIndex(static_cast<uint8>(stat)));
            writer->WriteIdentifierPrefix("values");
            writer->WriteArrayStart();
            writer->WriteValue(focusedUnit->GetStatComponent()->GetSkillBaseValue(stat));
            writer->WriteValue(focusedUnit->GetStatComponent()->GetSkillAdjValue(stat));
            writer->WriteArrayEnd();
            writer->WriteObjectEnd();
         }
      }
      writer->WriteArrayEnd();
      writer->Close();
      UpdateInformation("updateStatInfo", heroInfoString);
   } else
      UE_LOG(LogTemp, Error, TEXT("Somehow stat browser widget was told to read data from a null or non-hero"));
}

void UStatgraphWidget::ShowVitals()
{
   if(const auto focusedUnit = CheckIfFocusedUnitHero(); focusedUnit) {
      FString      heroInfoString;
      auto         writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&heroInfoString);
      const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("EVitals"), true);

      writer->WriteArrayStart();
      for(int i = 0; i < static_cast<int>(EVitals::Count); ++i) {
         const EVitals vit = static_cast<EVitals>(i);
         writer->WriteObjectStart();
         writer->WriteValue("statName", eBonus->GetNameStringByIndex(static_cast<uint8>(vit)));
         writer->WriteIdentifierPrefix("values");
         writer->WriteArrayStart();
         writer->WriteValue(focusedUnit->GetStatComponent()->GetVitalBaseValue(vit));
         writer->WriteValue(focusedUnit->GetStatComponent()->GetVitalAdjValue(vit));
         writer->WriteArrayEnd();
         writer->WriteObjectEnd();
      }
      writer->WriteArrayEnd();
      writer->Close();
      UpdateInformation("updateStatInfo", heroInfoString);

   } else
      UE_LOG(LogTemp, Error, TEXT("Somehow stat browser widget was told to read data from a null or non-hero"));
}

void UStatgraphWidget::SwapHero()
{
   if(const auto focusedUnit = CheckIfFocusedUnitHero(); focusedUnit) {
      FString      heroInfoString;
      auto         writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&heroInfoString);
      const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("EVitals"), true);

      writer->WriteObjectStart();
      writer->WriteValue("heroName", focusedUnit->GetGameName().ToString());
      writer->WriteObjectEnd();
      writer->Close();
      UpdateInformation("updateName", heroInfoString);
   } else
      UE_LOG(LogTemp, Error, TEXT("Somehow stat browser widget was told to read data from a null or non-hero"));
}

void UStatgraphWidget::UpdateStat(const FGameplayAttribute& attributeModified, float& newAttributeValue, AUnit* unitAffected)
{
   CreateAndSendStatUpdate(attributeModified, newAttributeValue, unitAffected, "baseValue");
}

void UStatgraphWidget::UpdateBaseStat(const FGameplayAttribute& attributeModified, float& newAttributeValue, AUnit* unitAffected)
{
   CreateAndSendStatUpdate(attributeModified, newAttributeValue, unitAffected, "adjValue");
}

void UStatgraphWidget::CreateAndSendStatUpdate(const FGameplayAttribute& attributeModified, float& newAttributeValue, AUnit* unitAffected, const FString& keyName)
{
   if(cpcRef->GetWidgetToggler()->IsWidgetOnScreen(EHUDs::HS_Character)) { // Only send an update to the browser if it is on screen
      if(const auto focusedUnit = CheckIfFocusedUnitHero(); focusedUnit) {
         if(focusedUnit == unitAffected) { // Make sure the stat that is updated is one pertaining to the unit we are looking at information for
            FString heroInfoString;
            auto    writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&heroInfoString);

            writer->WriteObjectStart();
            writer->WriteValue("statName", attributeModified.GetName());
            writer->WriteValue(keyName, newAttributeValue);
            writer->WriteObjectEnd();
            writer->Close();
            UpdateInformation("updateStat", heroInfoString);
         }
      }
   } else
      UE_LOG(LogTemp, Error, TEXT("Somehow stat browser widget was told to read data from a null or non-hero"));
}

FReply UStatgraphWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   Super::NativeOnMouseMove(InGeometry, InMouseEvent);
   if(UNLIKELY(GetBrowser() && !GetBrowser()->IsBrowserLoading())) {
      const FVector2D screenSpaceTolocalWidgetPosition{InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition())};
      GetBrowser()->TriggerMouseMove(screenSpaceTolocalWidgetPosition);
      return FReply::Handled();
   }
   return FReply::Unhandled();
}

const AUnit* UStatgraphWidget::CheckIfFocusedUnitHero() const
{
   const auto focusedUnit = cpcRef->GetBasePlayer()->GetFocusedUnit();
   if(LIKELY(focusedUnit && focusedUnit->GetClass()->IsChildOf(ABaseHero::StaticClass())))
      return focusedUnit;
   return nullptr;
}
