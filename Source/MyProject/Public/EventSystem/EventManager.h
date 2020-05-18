#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StoryStructs.h"
#include "EventManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChapterCompleted, FGameplayTag, chapterName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSectionCompleted, FGameplayTag, sectionName);

class ARTSGameMode;

UCLASS(BlueprintType, Blueprintable)
class MYPROJECT_API UEventManager : public UObject
{
   GENERATED_BODY()

   friend class UMyCheatManager;

   UEventManager();

#define GETTAG(tagName) FGameplayTag::RequestGameplayTag("Storyboard.Chapter." tagName)
   TArray<FGameplayTag> chapters{GETTAG("A Gnawing Sense of Emptiness"), GETTAG("I want to change the world"),
                                 GETTAG("You be you, I'll be me"),       GETTAG("Remember Elaine"),
                                 GETTAG("To Live means I must die"),     GETTAG("Bleeding Time"),
                                 GETTAG("New Years Sacrifice"),          GETTAG("Starry Transcendence"),
                                 GETTAG("Death is not the End"),         GETTAG("Fragment of Memories")};
   TArray<FGameplayTag> sections;
#undef GETTAG

   int currentChapter = 1;
   int currentSection = 1;

   UFUNCTION(BlueprintCallable, Category = "Progress")
   void SkipToEvent(int chapter, int section);

 public:
   void Init();

   const static int NUM_CHAPTERS = 10;
  
   UPROPERTY(BlueprintReadOnly, Category = "References")
   ARTSGameMode* gameModeRef = nullptr;

   /**List of all the chapters, sections, and triggers when moving to a new section*/
   UPROPERTY(EditAnywhere, Category = "Effect Data")
   UStorybook* storybook;

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnSectionCompleted OnSectionCompletedDelegate;

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnChapterCompleted OnChapterCompletedDelegate;

   UFUNCTION(BlueprintCallable, Category = "Progress")
   void MoveToNextSection();

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessor")
   FGameplayTag GetCurrentChapter() const { return storybook->chapters[currentChapter - 1].chapterTitle; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessor")
   FGameplayTag GetCurrentSection() const { return storybook->chapters[currentChapter - 1].sections[currentSection - 1].sectionTitle; }
};
