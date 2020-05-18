// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StatgraphWidget.h"


#include "AbilitySystemComponent.h"
#include "BaseCharacter.h"
#include "BaseHero.h"
#include "BasePlayer.h"
#include "BluEye.h"
#include "HUDManager.h"
#include "Unit.h"
#include "UserInput.h"

void UStatgraphWidget::NativeConstruct()
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
            writer->WriteValue(focusedUnit->GetSkillBaseValue(stat));
            writer->WriteValue(focusedUnit->GetSkillAdjValue(stat));
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
            writer->WriteValue(focusedUnit->GetSkillBaseValue(stat));
            writer->WriteValue(focusedUnit->GetSkillAdjValue(stat));
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
      for(auto mech : TEnumRange<EMechanics>()) {
         writer->WriteObjectStart();
         writer->WriteValue("statName", eBonus->GetNameStringByIndex(static_cast<uint8>(mech)));
         writer->WriteIdentifierPrefix("values");
         writer->WriteArrayStart();
         writer->WriteValue(focusedUnit->GetMechanicBaseValue(mech));
         writer->WriteValue(focusedUnit->GetMechanicAdjValue(mech));
         writer->WriteArrayEnd();
         writer->WriteObjectEnd();
      }

      for(auto stat : TEnumRange<EUnitScalingStats>()) {
         if(const auto& statName = eBonus2->GetNameStringByIndex(static_cast<uint8>(stat)); !statName.EndsWith("_Resist") && !statName.EndsWith("_Aff")) {
            writer->WriteObjectStart();
            writer->WriteValue("statName", eBonus2->GetNameStringByIndex(static_cast<uint8>(stat)));
            writer->WriteIdentifierPrefix("values");
            writer->WriteArrayStart();
            writer->WriteValue(focusedUnit->GetSkillBaseValue(stat));
            writer->WriteValue(focusedUnit->GetSkillAdjValue(stat));
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
      for(auto vit : TEnumRange<EVitals>()) {
         writer->WriteObjectStart();
         writer->WriteValue("statName", eBonus->GetNameStringByIndex(static_cast<uint8>(vit)));
         writer->WriteIdentifierPrefix("values");
         writer->WriteArrayStart();
         writer->WriteValue(focusedUnit->GetVitalBaseValue(vit));
         writer->WriteValue(focusedUnit->GetVitalAdjValue(vit));
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
   if(cpcRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Character)) { // Only send an update to the browser if it is on screen
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
   auto focusedUnit = cpcRef->GetBasePlayer()->focusedUnit;
   if(LIKELY(focusedUnit && focusedUnit->GetClass()->IsChildOf(ABaseHero::StaticClass())))
      return focusedUnit;
   return nullptr;
}
