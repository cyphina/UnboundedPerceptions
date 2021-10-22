// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "CoreMisc.h"
#include "Json.h"
#include "DialogManager.h"

UDialogManager* UDialogManager::dialogManager = nullptr;

UDialogManager::UDialogManager()
{
   static ConstructorHelpers::FObjectFinder<UDataTable> SpellLookupTableFinder(TEXT("/Game/RTS_Tutorial/Tables/Dialogs/Dialoglist"));
   if(SpellLookupTableFinder.Object) dialogLookupTable = SpellLookupTableFinder.Object;
}

void UDialogManager::InitializeManager()
{
   check(!dialogManager);
   dialogManager = NewObject<UDialogManager>(GetTransientPackage(), NAME_None);
   dialogManager->AddToRoot();
}

TArray<FDialogData> UDialogManager::LoadDialog(const FName& rowName, const FString& contextString)
{
   int                 i          = 0;
   FString             strRowName = rowName.ToString();
   TArray<FDialogData> dialogData;

   while(FDialogLookupRow* rowData = dialogLookupTable->FindRow<FDialogLookupRow>(*(strRowName + FString::FromInt(++i)), contextString, false))
   {
      dialogData.Emplace(FDialogData(rowData->nextDialogue, rowData->text, rowData->actor));
   }
#if UE_EDITOR
   if(dialogData.Num() == 0) UE_LOG(LogTemp, Warning, TEXT("Error, no dialog loaded from row %s!"), *strRowName);
#endif
   return dialogData;
}

#undef LOCTEXT_NAMESPACE
