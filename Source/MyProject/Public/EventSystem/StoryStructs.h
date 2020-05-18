#pragma once

#include "Trigger.h"
#include "GameplayTags.h"
#include "StoryStructs.generated.h"

USTRUCT(BlueprintType, NoExport)
struct FUpSection {
   FUpSection();

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Section")
   FGameplayTag sectionTitle;

   /**Triggers to be set when the section begins*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Section")
   TArray<FTriggerData> triggers;
};

USTRUCT(BlueprintType, NoExport)
struct FChapter {
   FChapter();

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chapter")
   FGameplayTag chapterTitle;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chapter")
   TArray<FUpSection> sections;
};

UCLASS()
class MYPROJECT_API UStorybook : public UDataAsset
{
   GENERATED_BODY()

 public:
   UPROPERTY(EditAnywhere, Category = "Storybook")
   TArray<FChapter> chapters;
};
