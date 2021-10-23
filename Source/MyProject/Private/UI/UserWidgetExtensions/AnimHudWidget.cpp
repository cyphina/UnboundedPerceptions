#include "MyProject.h"
#include "AnimHudWidget.h"
#include "UMGSequencePlayer.h"

void UAnimHudWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
   Super::OnAnimationFinished_Implementation(Animation);
   if(!GetSequencePlayer(Animation)->IsPlayingForward()) {
      OnWidgetRemovedFromViewport();
      SetVisibility(ESlateVisibility::Collapsed);
   }
}
