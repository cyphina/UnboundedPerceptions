// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "Summon.h"
#include "DIRender.h"

void ASummon::BeginPlay()
{
   Super::BeginPlay();
   AAlly::SetEnabled(true); // Will add this unit to the proper bookkeeping data structures
}

void ASummon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
   Super::EndPlay(EndPlayReason);
   if (EndPlayReason == EEndPlayReason::Destroyed)
      Die();
}

void ASummon::SetEnabled(bool bEnabled)
{
   Super::SetEnabled(bEnabled);
   auto basePlayerRef = controllerRef->GetBasePlayer();
   bEnabled ? basePlayerRef->summons.Add(this) : basePlayerRef->summons.Remove(this);
}

void ASummon::Die_Implementation()
{
   Super::Die_Implementation();
   // Body is hidden in unit version of Die but reveal damage numbers
   TArray<UDIRender*> damageComponents;
   GetComponents<UDIRender>(damageComponents);
   for(auto x : damageComponents) {
      x->SetVisibility(true);
   }

   // We can destroy ally summon units when they die, but not heroes since we need to keep referencing them in memory for later usage
   // Delay destruction to show damage numbers
   SetLifeSpan(2.f);
}
