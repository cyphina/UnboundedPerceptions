#pragma once

#include "CoreMinimal.h"
#include "DialogStructs.generated.h"

USTRUCT(BlueprintType, NoExport)
struct FDialogData {
   FDialogData() : nextDialogs(TArray<int>()), dialog(FText::GetEmpty()), actorName(FName()) {}
   FDialogData(const FDialogData& data) : nextDialogs(data.nextDialogs), dialog(data.dialog), actorName(data.actorName) {}
   FDialogData(TArray<int> nextDialogs, FText dialog, FName actorName) : nextDialogs(nextDialogs), dialog(dialog), actorName(actorName) {}

   /**Indices of the next nodes that can be reached from this one*/
   UPROPERTY(BlueprintReadWrite, Category = "Dialog")
   TArray<int> nextDialogs;
   /**Dialog to be displayed when this node is reached*/
   UPROPERTY(BlueprintReadWrite, Category = "Dialog")
   FText dialog;
   /**Name of the speaker for the current node*/
   UPROPERTY(BlueprintReadWrite, Category = "Dialog")
   FName actorName;
};