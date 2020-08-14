// Created 8/7/20 5:53 PM

#include "MyProject.h"
#include "NullAttackAnim.h"

UNullAttackAnim::UNullAttackAnim()
{
   mockAnimationTimeline.SetTimelineLength(2.f);
   SetupNotifyEvent(1.f);
}

void UNullAttackAnim::PlayAttackAnimation(float playRate)
{
   mockAnimationTimeline.SetPlayRate(playRate);
   mockAnimationTimeline.PlayFromStart();
}

void UNullAttackAnim::StopAttackAnimation()
{
   mockAnimationTimeline.Stop();
}

float UNullAttackAnim::GetMockAnimationLength() const
{
   return mockAnimationTimeline.GetTimelineLength();
}

void UNullAttackAnim::SetMockAnimationLength(float newLength)
{
   mockAnimationTimeline.SetTimelineLength(newLength);
}

void UNullAttackAnim::SetupNotifyEvent(float notifyTime)
{
   if(notifyTime < 0 || notifyTime > GetMockAnimationLength())
   {
      UE_LOG(LogTemp, Warning, TEXT("Invalid time. Either negative time or attempting to set keyframe time to be past end of timeline"));
      return;
   }

   FOnTimelineEvent onAttackNotify;
   onAttackNotify.BindUFunction(this, "AttackNotify");
   mockAnimationTimeline.AddEvent(notifyTime, onAttackNotify);
}

void UNullAttackAnim::SetAttackEventTime(float newTime)
{
   mockAnimationTimeline = FTimeline();
   SetupNotifyEvent(newTime);
}

void UNullAttackAnim::AttackNotify() const
{
   OnAttackNotifyEvent.Broadcast();
}

void UNullAttackAnim::NativeUpdateAnimation(float DeltaSeconds)
{
   Super::NativeUpdateAnimation(DeltaSeconds);
   mockAnimationTimeline.TickTimeline(DeltaSeconds);
}
