#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"
#include "ActionSlot.h"
#include "SkillSlot.generated.h"

class UMySpell;
class UESkillContainer;

UCLASS()
class MYPROJECT_API USkillSlot : public UActionSlot
{
   GENERATED_BODY()

   FTimeline cdTimeline;

   static UMaterialInterface* matInstance;

   UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
   int cdTimer;

 public:
   UPROPERTY(BlueprintReadWrite) // Reference to skill container which holds this slot
   UESkillContainer* eSkillContainer;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) // Default image for an empty slot
   UTexture2D* defaultSlotTexture;

   UPROPERTY(BlueprintReadWrite, Category = "Action", Meta = (BindWidget)) // Cooldown radial coloring
   UImage* imgCD;

   UPROPERTY(BlueprintReadWrite, Category = "Action", Meta = (BindWidget)) // Cooldown timer
   UTextBlock* txtCDTime;

   UPROPERTY(BlueprintReadWrite) // make sure to set this up in bp -- cooldown shadow material
   UMaterialInstanceDynamic* cdDMatInst;

   UPROPERTY(BlueprintReadWrite) // make sure to set this up in bp -- image material
   UMaterialInstanceDynamic* imageDMatInst;

   USkillSlot(const FObjectInitializer& o);

   void NativeConstruct() override;

   void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

   /**Function that is called when this slot is activated*/
   void PerformAction();

   /**Used to create the effect of the skillslot being filled as it goes off cooldown*/
   UFUNCTION(BlueprintCallable)
   void TickTimeline();

   /**Can be used if this slot is replaced with another skill to update the timeline length*/
   UFUNCTION(BlueprintCallable, Category = "Binding Functions")
   void UpdateTimelineCD(float newDuration);

   /**When the cd visuals need to be hided (time and image mostly)*/
   UFUNCTION(BlueprintCallable)
   void HideCDVisuals();

   /**When the cd visuals need to be shown (time and image mostly)*/
   UFUNCTION(BlueprintCallable)
   void ShowCDVisuals();

   /*Special playtimeline function used to keep playing at the point at which a spell is on cd.  Used if skill on cd swapped to new slot*/
   UFUNCTION(BlueprintCallable, Category = "Timeline Exposure")
   void PlayTimeline(float startingPoint);

   /**Used when changing the skill to a new one*/
   UFUNCTION(BlueprintCallable, Category = "Update skills")
   void UpdateSkillSlot(TSubclassOf<UMySpell> spell);

   void SetImage(UTexture2D* image) override;

   /**Used to generate tooltip description*/
   void ShowDesc(UToolTipWidget* tooltip) override;

   void OnBtnClick() override;
};
