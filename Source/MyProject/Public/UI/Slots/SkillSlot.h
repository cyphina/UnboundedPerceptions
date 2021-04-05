#pragma once

#include "CoreMinimal.h"
#include "ActionSlot.h"
#include "SkillSlot.generated.h"

class URTSAbilitySystemComponent;
class UMySpell;

UCLASS(meta = (DisableNativeTick))
class MYPROJECT_API USkillSlot : public UActionSlot
{
   GENERATED_BODY()

 public:
   USkillSlot(const FObjectInitializer& o);

<<<<<<< HEAD
   /* Resumes cooldown effect for the new skill if a new skill is swapped into this skill slot that is on CD */
   void ShowCooldown();

   /**
    * Used when changing the skill to a new one.
    * Can be triggered via a spell effect or via the player rearranging their actionbar
    */
   UFUNCTION(BlueprintCallable, Category = "Update skills")
   void UpdateSkillSlot(TSubclassOf<UMySpell> spell);

 protected:
   void NativeConstruct() override;
=======
   /**
    * Used when changing the skill to a new one.
    * Can be triggered via a spell effect or via the player rearranging their actionbar
    */
   UFUNCTION(BlueprintCallable, Category = "Update skills")
   void UpdateSkillSlot(TSubclassOf<UMySpell> spell);

   /** Resumes cooldown effect for the new skill if a new skill is swapped into this skill slot that is on CD */
   void ShowCooldown();

 protected:
   void NativeOnInitialized() override;
   void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
   void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
   bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
>>>>>>> componentrefactor

   // Cooldown radial coloring
   UPROPERTY(BlueprintReadWrite, Category = "Action", Meta = (BindWidget))
   UImage* Image_CD;

   // Cooldown timer
   UPROPERTY(BlueprintReadWrite, Category = "Action", Meta = (BindWidget))
   UTextBlock* Text_CDTime;

   // Default image for an empty slot
   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
   UTexture2D* defaultSlotTexture;

   // Make sure to set this up in bp -- cooldown shadow material
   UPROPERTY(BlueprintReadWrite)
   UMaterialInstanceDynamic* cdDMatInst;

   // Make sure to set this up in bp -- image material
   UPROPERTY(BlueprintReadWrite)
   UMaterialInstanceDynamic* imageDMatInst;

 private:
   /**
    * @brief Used to create the effect of the skill slot being filled as it goes off cooldown
    */
   UFUNCTION()
   void UpdateCD();

   /**When the cd visuals need to be hided (time and image mostly)*/
<<<<<<< HEAD
   UFUNCTION(BlueprintCallable)
   void OnCDFinished();

   /**When the cd visuals need to be shown (time and image mostly)*/
   UFUNCTION(BlueprintCallable)
=======
   UFUNCTION()
   void OnCDFinished();

   /**When the cd visuals need to be shown (time and image mostly)*/
   UFUNCTION()
>>>>>>> componentrefactor
   void ShowCDVisuals() const;

   URTSAbilitySystemComponent* GetOwningAbilityComponent() const;

   /**Used to generate tooltip description*/
   void ShowDesc(UToolTipWidget* tooltip) override;

   void ResetSkillSlot();

   void SetSlotImage(UTexture2D* image) override;

<<<<<<< HEAD
   void OnBtnClick() override;

=======
>>>>>>> componentrefactor
   /**
    * @brief Timer used to update the slot radial effect and cooldown text
    */
   FTimerHandle cooldownProgressTimer;

<<<<<<< HEAD
   static UMaterialInterface* matInstance;
=======
   static UMaterialInterface* cdMatInstance;
   static UMaterialInterface* skillMatInstance;
>>>>>>> componentrefactor
};
