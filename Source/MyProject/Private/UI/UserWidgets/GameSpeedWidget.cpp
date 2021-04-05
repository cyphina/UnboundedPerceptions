// Created 8/30/20 11:18 AM

#include "MyProject.h"
#include "GameSpeedWidget.h"
#include "Button.h"
#include "RTSGameState.h"

void UGameSpeedWidget::NativeOnInitialized()
{
   Btn_IncGameSpeed->OnClicked.AddDynamic(this, &UGameSpeedWidget::IncreaseGameSpeed);
   Btn_IncGameSpeed->OnClicked.AddDynamic(this, &UGameSpeedWidget::DecreaseGameSpeed);
   gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
}

void UGameSpeedWidget::IncreaseGameSpeed()
{
   gameStateRef->OnGameSpeedUpdated().Broadcast(gameSpeeds[++speedIndex]);
}

void UGameSpeedWidget::DecreaseGameSpeed()
{
   gameStateRef->OnGameSpeedUpdated().Broadcast(gameSpeeds[--speedIndex]);
}
