#include "Trigger.h"
#include "GameplayTags.h"
#include "StoryStructs.generated.h"

USTRUCT(BlueprintType, NoExport)
struct FUpSection {
   FUpSection();

   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   FGameplayTag sectionTitle;

   /**Triggers to be set when the section begins*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   TArray<FTriggerData> triggers;
};

USTRUCT(BlueprintType, NoExport)
struct FChapter {
   FChapter();

   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   FGameplayTag chapterTitle;

   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   TArray<FUpSection> sections;
};

UCLASS()
class MYPROJECT_API UStorybook : public UDataAsset
{
   GENERATED_BODY()

 public:
   UPROPERTY(EditAnywhere)
   TArray<FChapter> chapters;
};
