// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogManager.generated.h"

/**
 * Claass for managing dialogue, including changes due to events, and loading dialog
 * Purpose for centralizing these functions is so they can be used throughout different game systems that may need dialog, like cutscenes, talking, etc.
 */

template<class T>
class DialogGraph;

USTRUCT(BlueprintType)
struct FDialogData
{
	GENERATED_USTRUCT_BODY()

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

USTRUCT(Blueprintable)
struct FDialogLookupRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	TArray<int>				nextDialogue; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	FText					text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	FName					actor;
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
		if (dialogManager == nullptr)
		{
			InitializeManager();
		}
		return *dialogManager;
	}


	//Load up dialogue tree
	//UFUNCTION(BlueprintCallable, Category = "Dialog Handling")
	//void			MakeDialogTree(FString serializedDialogString);
	
	/**Load up serialized dialogue string from file
	@param contextString - A string with information about the caller.*/ 
	UFUNCTION(BlueprintCallable, Category = "File Handling")
	TArray<FDialogData> 			LoadDialog(const FName& rowName, FString contextString);

private:
	static UDialogManager*			dialogManager;
	
	static void					InitializeManager();

	void							SetupDialogs();

	UDataTable*						dialogLookupTable;
};
