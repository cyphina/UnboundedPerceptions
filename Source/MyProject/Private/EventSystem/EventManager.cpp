// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "EventManager.h"
#include "RTSGameMode.h"

UEventManager::UEventManager()
{
   static ConstructorHelpers::FObjectFinder<UStorybook> SpellLookupTableFinder(TEXT("/Game/RTS_Tutorial/Blueprints/Events/StorybookData"));
   if (SpellLookupTableFinder.Object) storybook = SpellLookupTableFinder.Object;
}

void UEventManager::Init()
{
   gameModeRef = Cast<ARTSGameMode>(GetOuter());
}

void UEventManager::MoveToNextSection()
{
   if (currentSection >= storybook->chapters[currentChapter].sections.Num()) {
      ++currentChapter;
      currentSection = 1;
      OnChapterCompletedDelegate.Broadcast(GetCurrentChapter());
      checkf(currentChapter <= 10, TEXT("Why are you past the last chapter hacker!"));
   } else {
      ++currentSection;
   }
   OnSectionCompletedDelegate.Broadcast(GetCurrentSection());

   for (FTriggerData trigger : storybook->chapters[currentChapter - 1].sections[currentSection - 1].triggers) {
      gameModeRef->GetTriggerManager()->ActivateTrigger(trigger);
   }
   // currentSection = currentSection % chapters[currentChapter].sections.Num() + 1;
}
