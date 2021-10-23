#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogStructs.h"
#include "DialogManager.generated.h"

/**
 * Class which interfaces the dialog table(s).  There may be multiple tables to divide up what is loaded since there's so much dialog per chapter.
 * Most of the time, fetching the dialog will not be through this singleton, but through triggers which also display the fetched lines.
 */

USTRUCT(Blueprintable)
struct FDialogLookupRow : public FTableRowBase {
   GENERATED_USTRUCT_BODY()

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
   TArray<int> nextDialogue;
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog", meta = (MultiLine = true))
   FText text;
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
   FName actor;
};

UCLASS()
class MYPROJECT_API UDialogManager : public UObject
{
   GENERATED_BODY()

 public:
   UDialogManager();
   ~UDialogManager() = default;

   FORCEINLINE static UDialogManager& Get()
   {
      if (dialogManager == nullptr) { InitializeManager(); }
      return *dialogManager;
   }

   /**Load up dialogue string from the table:
   @param contextString - A string with information about the caller.  Haven't found much use of this, usually I just pass an empty string*/
   UFUNCTION(BlueprintCallable, Category = "File Handling")
   TArray<FDialogData> LoadDialog(const FName& rowName, const FString& contextString);

 private:
   static UDialogManager* dialogManager;

   static void InitializeManager();

   UPROPERTY()
   UDataTable* dialogLookupTable;
};
