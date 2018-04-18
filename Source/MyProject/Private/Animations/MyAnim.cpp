// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyAnim.h"

void UMyAnim::UpdateAnimationProperties()
{
	APawn* pawn = TryGetPawnOwner();
	if(pawn)
	{
		//Update our falling property
		if(pawn->GetMovementComponent())
		bIsFalling = pawn->GetMovementComponent()->IsFalling();
		//Update our movement speed
		movementSpeed = pawn->GetVelocity().Size();
	}
}

void UMyAnim::NativeUpdateAnimation(float deltaSeconds)
{
	UpdateAnimationProperties();
}
