// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "Summon.h"


void ASummon::BeginPlay()
{
	Super::BeginPlay();
	controllerRef->GetBasePlayer()->summons.Add(this);
}


