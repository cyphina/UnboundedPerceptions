// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacterMovementComponent.generated.h"

/**
 * Custom movement component
 */
UCLASS(HideCategories=("Character Movement (General Settings)", "Character Movement: Swimming", "Character Movement: Flying"))
class MYPROJECT_API UMyCharacterMovementComponent : public UCharacterMovementComponent
{
   GENERATED_UCLASS_BODY()

 protected:
   virtual void InitializeComponent() override;
   virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
