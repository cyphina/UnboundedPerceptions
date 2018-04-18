// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "SkillSlot.h"
#include "ActionSlot.h"
#include "../ESkillContainer.h"
#include "UserInput.h"
#include "../HUDManager.h"
#include "../UserWidgets/MainWidget.h"
#include "SpellSystem/MySpell.h"
#include "WorldObjects/Ally.h"
#include "AbilitySystemComponent.h"

USkillSlot::USkillSlot(const FObjectInitializer& o) : UUserWidget(o)
{
	//static ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Game/RTS_Tutorial/HUDs/ActionUI/StandardLinear"));
	//checkf(curve.Object, TEXT("Curve for ability timelines not found!"))
	FOnTimelineEvent e;
	e.BindUFunction(this, "TickTimeline");
	cdTimeline.SetTimelinePostUpdateFunc(e);
	FOnTimelineEvent e2;
	e2.BindUFunction(this, "HideCDVisuals");
	cdTimeline.SetTimelineFinishedFunc(e2); //we need this when it stops to hide the cd thing
	cdTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	cdTimeline.SetLooping(false);
}

void USkillSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(cdTimeline.IsPlaying())
	{
		cdTimeline.TickTimeline(InDeltaTime);
	}
}

//void USkillSlot::PerformAction()
//{
//	eSkillContainer->GetAllyRef()->spellIndex = actionSlotRef->slotIndex;
//	eSkillContainer->GetAllyRef()->BeginCastSpell(eSkillContainer->GetAllyRef()->abilities[actionSlotRef->slotIndex]);
//}

void USkillSlot::TickTimeline()
{	
	float cdTimeRemaining = eSkillContainer->GetAllyRef()->abilities[actionSlotRef->slotIndex].GetDefaultObject()->GetCooldownTimeRemaining(eSkillContainer->GetAllyRef()->GetAbilitySystemComponent()->AbilityActorInfo.Get());
#if UE_EDITOR
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, FString::FromInt(cdTimeRemaining));
#endif
	cdTimer = cdTimeRemaining;
	cdDMatInst->SetScalarParameterValue("Percent", cdTimeRemaining/cdTimeline.GetTimelineLength()); //use cdTimeRemaining for float precision
}

void USkillSlot::UpdateTimelineCD(float newDuration)
{
	cdTimeline.SetTimelineLength(newDuration);
}

void USkillSlot::PlayTimeline(float startingPoint)
{
	if (startingPoint == 0)
	{
		cdTimeline.PlayFromStart();
	}
	else {
		cdTimeline.SetNewTime(cdTimeline.GetTimelineLength() - startingPoint);
#if UE_EDITOR
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::FromInt(cdTimeline.GetTimelineLength() - startingPoint));
#endif
		
		cdTimeline.Play();
	}
	ShowCDVisuals();
}

void USkillSlot::UpdateSkillSlot(TSubclassOf<UMySpell> spellClass)
{
	if (IsValid(spellClass))
	{
		eSkillContainer->GetAllyRef()->abilities[actionSlotRef->slotIndex] = spellClass;
		UMySpell* spellObject = spellClass.GetDefaultObject();
		SetImage(spellObject->spellDefaults.image); //update the image

		UpdateTimelineCD(spellObject->GetCDDuration(eSkillContainer->GetAllyRef()->GetAbilitySystemComponent())); //update timeline duration if someone plays it
		
		if (spellObject->GetCooldownTimeRemaining(eSkillContainer->GetAllyRef()->GetAbilitySystemComponent()->AbilityActorInfo.Get()) > 0) //if on cd
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("Skill off CD!"));
			PlayTimeline(spellObject->GetCooldownTimeRemaining(eSkillContainer->GetAllyRef()->GetAbilitySystemComponent()->AbilityActorInfo.Get())); //play out the rest of the timeline for its cd duration
		}
		else
		{
			cdTimeline.Stop(); //stop the timeline so it doens't tick anymore 
			HideCDVisuals(); //and hide skill indicators
		}
	}
	else
		SetImage(nullptr); 
}


