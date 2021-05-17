#pragma once

#include "MyUserWidget.h"
#include "WidgetAnimation.h"
#include "AnimHudWidget.generated.h"

/**
 * Inherit from ths if we want our widget to play an animation when it is toggled on and off from the screen
 */
UCLASS()
class MYPROJECT_API UAnimHudWidget : public UMyUserWidget
{
   GENERATED_BODY()

 public:
   UWidgetAnimation* GetAnim() const { return anim; }

 protected:
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   UWidgetAnimation* anim;

   void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
