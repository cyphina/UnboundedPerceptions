#pragma once

#include "CoreMinimal.h"
#include "DialogStructs.generated.h"

USTRUCT(BlueprintType, NoExport)
struct FDialogData
{
	FDialogData() : nextDialogs(TArray<int>()), dialog(FText::GetEmpty()), actorName(FName()) {}
	FDialogData(const FDialogData& data) : nextDialogs(data.nextDialogs), dialog(data.dialog), actorName(data.actorName) {}
	FDialogData(TArray<int> nextDialogs, FText dialog, FName actorName) : nextDialogs(nextDialogs), dialog(dialog), actorName(actorName) {}

	UPROPERTY(BlueprintReadOnly, Category = "Dialog")
	TArray<int> nextDialogs;
	UPROPERTY(BlueprintReadOnly, Category = "Dialog")
	FText dialog;
	UPROPERTY(BlueprintReadOnly, Category = "Dialog")
	FName actorName;
};