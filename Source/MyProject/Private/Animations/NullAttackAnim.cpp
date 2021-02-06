// Created 8/7/20 5:53 PM

#include "MyProject.h"
#include "NullAttackAnim.h"

#include "RTSGlobalCVars.h"

UNullAttackAnim::UNullAttackAnim()
{
   SetMockAnimationLength(2.f);
   SetAttackEventTime(1.f);
}

void UNullAttackAnim::PlayAttackAnimation(float playRate)
{
   if(AttackCVars::bPrintAttackTimings)
   {
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, "Attack Animation Started!");
   }

   SetupHitTimer();
   GetWorld()->GetTimerManager().SetTimer(
       mockAnimationTimer,
       [this]() {
          SetupHitTimer();
          OnAttackAnimFinished().Broadcast();
       },
       timelineLength, true);
}

void UNullAttackAnim::StopAttackAnimation()
{
   GetWorld()->GetTimerManager().ClearTimer(mockAnimationTimer);
   GetWorld()->GetTimerManager().ClearTimer(hitNotifyTimer);
}

float UNullAttackAnim::GetMockAnimationLength() const
{
   return timelineLength;
}

void UNullAttackAnim::SetMockAnimationLength(float newLength)
{
   timelineLength = newLength;
}

void UNullAttackAnim::SetAttackEventTime(float newTime)
{
   if(newTime < 0 || newTime > GetMockAnimationLength())
   {
      UE_LOG(LogTemp, Warning, TEXT("Invalid time. Either negative time or attempting to set keyframe time to be past end of timeline"));
      return;
   }

   hitNotifyTime = newTime;
}

void UNullAttackAnim::AttackNotify()
{
   if(AttackCVars::bPrintAttackTimings)
   {
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, "Attack Hit Notification!");
   }

   OnAttackNotifyEvent.Broadcast();
}

void UNullAttackAnim::SetupHitTimer()
{
   GetWorld()->GetTimerManager().SetTimer(hitNotifyTimer, this, &UNullAttackAnim::AttackNotify, hitNotifyTime, false);
}
