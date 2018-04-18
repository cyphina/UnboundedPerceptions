#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "EventManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSectionCompleted, FGameplayTag, sectionName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChapterCompleted, FGameplayTag, chapterName);

USTRUCT(BlueprintType, NoExport)
struct FChapter
{
	FChapter(FGameplayTag chapterTitle, TArray<FGameplayTag> sectionTitles) : chapterTitle(chapterTitle), sectionTitles(sectionTitles) {}
	FGameplayTag chapterTitle;
	TArray<FGameplayTag> sectionTitles;
};

/*Manager class for all large scale events*/

UCLASS()
class MYPROJECT_API UEventManager : public UActorComponent
{
	GENERATED_BODY()

	#define GETTAG(tagName) FGameplayTag::RequestGameplayTag(tagName)
	TArray<FGameplayTag> 	chapters {GETTAG("A Gnawing Sense of Emptiness"), GETTAG("I want to change the world"), GETTAG("You be you, I'll be me"),
									  GETTAG("Remember Elaine"), GETTAG("To Live means I must die"), GETTAG("Bleeding Time"), GETTAG("New Years Sacrifice"),
									  GETTAG("Goodbye"), GETTAG("Death is not the End"), GETTAG("Memory's Fragment")};
	TArray<FGameplayTag>	sections;
	#undef GETTAG
	int						currentChapter = 0;
	int						currentSection = 0;
	 
public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
	FOnSectionCompleted		OnSectionCompletedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
	FOnChapterCompleted		OnChapterCompletedDelegate;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessor")
	FGameplayTag			GetCurrentChapter() const { return chapters[currentChapter]; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessor")
	FGameplayTag			GetCurrentSection() const { return sections[currentSection]; }

};
