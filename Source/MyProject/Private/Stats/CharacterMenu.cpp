// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "CharacterMenu.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "WorldObjects/BaseHero.h"

bool UCharacterMenu::OnWidgetAddToViewport_Implementation()
{
   Super::OnWidgetAddToViewport_Implementation();
   baseHeroRef = CPC->GetBasePlayer()->selectedHeroes[0];
   return baseHeroRef ? true : false;
}
