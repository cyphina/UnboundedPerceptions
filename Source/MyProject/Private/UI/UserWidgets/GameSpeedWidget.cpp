// Created 8/30/20 11:18 AM

#include "MyProject.h"
#include "GameSpeedWidget.h"
#include "Button.h"
#include "RTSGameState.h"
#include "TextBlock.h"

void UGameSpeedWidget::NativeOnInitialized()
{
   Btn_IncGameSpeed->OnClicked.AddDynamic(this, &UGameSpeedWidget::IncreaseGameSpeed);
   Btn_DecGameSpeed->OnClicked.AddDynamic(this, &UGameSpeedWidget::DecreaseGameSpeed);
   // TODO: Should probably wait till this is replicated and pass it down from AUserInput
   gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
}

void UGameSpeedWidget::IncreaseGameSpeed()
{
   gameStateRef->OnGameSpeedUpdated().Broadcast(gameSpeeds[++speedIndex]);
   Text_GameSpeed->SetText(FText::AsNumber(speedIndex));
}

void UGameSpeedWidget::DecreaseGameSpeed()
{
   gameStateRef->OnGameSpeedUpdated().Broadcast(gameSpeeds[--speedIndex]);
   Text_GameSpeed->SetText(FText::AsNumber(speedIndex));
}
