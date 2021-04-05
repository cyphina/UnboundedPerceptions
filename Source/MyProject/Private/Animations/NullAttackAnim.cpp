// Created 8/7/20 5:53 PM

#include "MyProject.h"
#include "NullAttackAnim.h"

<<<<<<< HEAD
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
=======
#include "RTSGlobalCVars.h"

UNullAttackAnim::UNullAttackAnim()
{
   SetMockAnimationLength(2.f);
   SetAttackEventTime(1.f);
}

void UNullAttackAnim::PlayAttackAnimation(ACharacter* characterToPlayMontageOn, float playRate)
{
   if(AttackCVars::bPrintAttackTimings)
   {
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, "Attack Animation Started!");
   }

   SetMockAnimationLength(2.f / playRate);
   SetAttackEventTime(1.f / playRate);

   SetupHitTimer();
   GetWorld()->GetTimerManager().SetTimer(
       mockAnimationTimer,
       [this]() {
          SetupHitTimer();
       },
       timelineLength, true);
}

void UNullAttackAnim::StopAttackAnimation(ACharacter* characterToStopMontageOn)
{
   GetWorld()->GetTimerManager().ClearTimer(mockAnimationTimer);
   GetWorld()->GetTimerManager().ClearTimer(hitNotifyTimer);
>>>>>>> componentrefactor
}

float UNullAttackAnim::GetMockAnimationLength() const
{
<<<<<<< HEAD
   return mockAnimationTimeline.GetTimelineLength();
=======
   return timelineLength;
>>>>>>> componentrefactor
}

void UNullAttackAnim::SetMockAnimationLength(float newLength)
{
<<<<<<< HEAD
   mockAnimationTimeline.SetTimelineLength(newLength);
}

void UNullAttackAnim::SetupNotifyEvent(float notifyTime)
{
   if(notifyTime < 0 || notifyTime > GetMockAnimationLength())
=======
   timelineLength = newLength;
}

void UNullAttackAnim::SetAttackEventTime(float newTime)
{
   if(newTime < 0 || newTime > GetMockAnimationLength())
>>>>>>> componentrefactor
   {
      UE_LOG(LogTemp, Warning, TEXT("Invalid time. Either negative time or attempting to set keyframe time to be past end of timeline"));
      return;
   }

<<<<<<< HEAD
   FOnTimelineEvent onAttackNotify;
   onAttackNotify.BindUFunction(this, "AttackNotify");
   mockAnimationTimeline.AddEvent(notifyTime, onAttackNotify);
}

void UNullAttackAnim::SetAttackEventTime(float newTime)
{
   mockAnimationTimeline = FTimeline();
   SetupNotifyEvent(newTime);
=======
   hitNotifyTime = newTime;
>>>>>>> componentrefactor
}

void UNullAttackAnim::AttackNotify()
{
<<<<<<< HEAD
   OnAttackNotifyEvent.Broadcast();
}

void UNullAttackAnim::NativeUpdateAnimation(float DeltaSeconds)
{
   Super::NativeUpdateAnimation(DeltaSeconds);
   mockAnimationTimeline.TickTimeline(DeltaSeconds);
=======
   if(AttackCVars::bPrintAttackTimings)
   {
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, "Attack Hit Notification!");
   }

   OnAttackNotifyEvent.Broadcast();

   // Set this up here else we have a situation where the animation finishing puts our attacks on CD and they can't be unfroze by the hit event.
   // Attack timer starts on the end of the hit in dota, but for animations it starts at the very end of the animation (after all hits).
   OnAttackAnimFinished()->Broadcast();
}

void UNullAttackAnim::SetupHitTimer()
{
   if(UWorld* world = GetWorld())
   {
      world->GetTimerManager().SetTimer(hitNotifyTimer, this, &UNullAttackAnim::AttackNotify, hitNotifyTime, false);
   }
>>>>>>> componentrefactor
}
