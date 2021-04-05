#pragma once
#include "StatEnums.h"
#include "UI/UserWidgetExtensions/AnimHudWidget.h"
#include "CharacterMenu.generated.h"

/**
 * Widget for seeing character stats and allocating points towards attributes.
 */

class AUnit;
struct FGameplayAttribute;
class UTextBlock;
class UStatgraphWidget;
class UButton;
class ABaseHero;
class UVerticalBox;
class UAttributePointSpenderWidget;

UCLASS()
class MYPROJECT_API UCharacterMenu : public UAnimHudWidget
{
   GENERATED_BODY()

public:
   ABaseHero* GetHeroWithInfoDisplayed() const { return baseHeroRef; };

protected:
   UFUNCTION(BlueprintImplementableEvent)
   void SetupCharacterPortrait(ABaseHero* heroToPortray);
   
   void NativeOnInitialized() override;
   bool OnWidgetAddToViewport_Implementation() override;
   void OnWidgetRemovedFromViewport_Implementation() override;
   void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
   void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
   
   UPROPERTY(Meta=(BindWidget))
   UStatgraphWidget* Widget_StatGraph;

   UPROPERTY(Meta=(BindWidget))
   UVerticalBox* Pan_CharBio;

   UPROPERTY(Meta=(BindWidget))
   UVerticalBox* Pan_Attributes;

   UPROPERTY(Meta=(BindWidget))
   UVerticalBox* Pan_Social;

   UPROPERTY(Meta=(BindWidget))
   UVerticalBox* Pan_Party;

   UPROPERTY(Meta=(BindWidget))
   UVerticalBox* Pan_OtherM;

   UPROPERTY(Meta=(BindWidget))
   UTextBlock* Text_Name;
   
   UPROPERTY(Meta=(BindWidget))
   UTextBlock* Text_Level;

   UPROPERTY(Meta=(BindWidget))
   UTextBlock* Text_Money;

   UPROPERTY(Meta=(BindWidget))
   UTextBlock* Text_EXP;

   UPROPERTY(Meta=(BindWidget))
   UTextBlock* Text_AttPoints;
   
   UPROPERTY(Meta=(BindWidget))
   UButton* Btn_Affinity;

   UPROPERTY(Meta=(BindWidget))
   UButton* Btn_Resist;

   UPROPERTY(Meta=(BindWidget))
   UButton* Btn_Mechanics;

   UPROPERTY(Meta=(BindWidget))
   UButton* Btn_Vitals;

   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<UAttributePointSpenderWidget> AttributePointerWidgetClass;

private:
   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "References")
   ABaseHero* baseHeroRef = nullptr;

   UFUNCTION()
   void ShowAffinities();

   UFUNCTION()
   void ShowDefenses();

   UFUNCTION()
   void ShowVitals();

   UFUNCTION()
   void ShowMechanics();

   void OnBaseStatsUpdated(const FGameplayAttribute& updatedBaseAttribute, float newValue, AUnit* updatedUnit);

   void OnStatsUpdated(const FGameplayAttribute& updatedAttribute, float newValue, AUnit* updatedUnit);
   
   void OnAttributePointAllocated(ABaseHero* heroAllocating, EAttributes upgradedStat, bool bAllocated);

   void InitialHeroValueSetup(ABaseHero* heroRef);
   
   void OnMoneyGained(int moneyGained);

   void OnEXPGained(int expGained);

   void OnHeroLevelUp(ABaseHero* heroLevelingUp);
   
   void CreateAttPointSpendingWidgets();
};
