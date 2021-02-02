// Created 2/1/21 9:14 AM

#pragma once
#include "Object.h"
#include "ActionSlotStyle.generated.h"

/**
 * @brief Allows us to style our action slots
 */
UCLASS(Abstract, Blueprintable, ClassGroup = UI)
class MYPROJECT_API UActionSlotStyle : public UObject
{
   GENERATED_BODY()

 public:
   UTexture2D*  GetDefaultSlotImage() const { return DefaultSlotImage; }
   const FSlateBrush& GetDefaultBrush() const { return DefaultBrush; }
   const FSlateBrush& GetPressedBrush() const { return PressedBrush; }
   const FSlateBrush& GetDisabledBrush() const { return DisabledBrush; }

 protected:
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
   UTexture2D* DefaultSlotImage;

   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
   FSlateBrush DefaultBrush;

   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
   FSlateBrush PressedBrush;

   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
   FSlateBrush DisabledBrush;
};
