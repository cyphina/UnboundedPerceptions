#include "MyProject.h"
#include "Minimap.h"

#include "BaseHero.h"
#include "BasePlayer.h"
#include "GoalActor.h"
#include "QuestManager.h"
#include "RTSGameMode.h"
#include "UserInput.h"

void UMinimap::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   GetWorld()->GetTimerManager().SetTimer(updateQuestDirectionArrowTimerHandle, [this]()
   {
      if(AActor* goalActor = CPC->GetGameMode()->questManager->GetGoalActor())
      {
         if(GetDistanceToGoal() > 10)
         {
            ToggleDirectionArrowVisibility(false);
         } else
         {
            ToggleDirectionArrowVisibility(true);
         }
         const FVector partyLeaderLocation           = CPC->GetBasePlayer()->GetHeroes()[0]->GetActorLocation();
         const FVector questGoalActorLocation        = goalActor->GetActorLocation();
         const FVector fromPartyLeaderToQuestGoalVec = partyLeaderLocation - questGoalActorLocation;
         UpdateDirectionArrow(FMath::RadiansToDegrees(FMath::Acos(FVector::ForwardVector.CosineAngle2D(fromPartyLeaderToQuestGoalVec))));
      }
   }, 0.1f, true, 0.f);
}

void UMinimap::OnQuestCompleted(AQuest* questCompleted)
{
}

void UMinimap::OnSubgoalSwitched(AQuest* quest, int goalIndex)
{
   if(GetDistanceToGoal() > 10)
   {
      ToggleDirectionArrowVisibility(true);
   } else
   {
      ToggleDirectionArrowVisibility(false);
   }
}

int UMinimap::GetDistanceToGoal()
{
   const ABaseHero* leaderRef             = CPC->GetBasePlayer()->GetHeroes()[0];
   const FVector    partyLeaderLocationXY = FVector(leaderRef->GetActorLocation().X, leaderRef->GetActorLocation().Y, 0);
   if(ARTSGameMode* gameModeRef = GetWorld()->GetAuthGameMode<ARTSGameMode>())
   {
      AActor*       goalActor           = gameModeRef->GetQuestManager()->GetGoalActor();
      const FVector goalActorLocationXY = FVector(goalActor->GetActorLocation().X, goalActor->GetActorLocation().Y, 0);
      return FMath::RoundToInt((partyLeaderLocationXY - goalActorLocationXY).Size2D() / 100);
   }
   return -1;
}
