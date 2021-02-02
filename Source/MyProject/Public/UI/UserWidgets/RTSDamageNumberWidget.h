// Created 1/31/21 02:28 PM

#pragma once
#include "UserWidget.h"
#include "RTSDamageNumberWidget.generated.h"

struct FUpDamage;

/** The block of text that appears whenever we trigger a damage event */
class UTextBlock;
UCLASS()
class MYPROJECT_API URTSDamageNumberWidget : public UUserWidget
{
   GENERATED_BODY()

public:
   void NativeOnInitialized() override;
   void SetDamageTextProps(const FUpDamage& damageInfo);
   
protected:
   UPROPERTY(Meta=(BindWidget))
   UTextBlock* damageText;

   UPROPERTY(Meta=(BindWidgetAnim))
   UWidgetAnimation* floatAndDissapearAnimation;

private:
   UFUNCTION()
   void OnFloatAnimationFinished();
};
