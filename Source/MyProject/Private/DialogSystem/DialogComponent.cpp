// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "GenericPlatformFile.h"
#include "DialogComponent.h"
#include "WorldObjects/WorldObject.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "DialogBox.h"
#include "GameplayTagContainer.h"

UDialogComponent::UDialogComponent()
{
   PrimaryComponentTick.bCanEverTick = false;
}

void UDialogComponent::BeginPlay()
{
   Super::BeginPlay();

   // Setup references and filepaths
   basePlayerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController())->GetBasePlayer();
   basePlayerRef->OnDialogLearned.AddDynamic(this, &UDialogComponent::UpdateDialogs);
   //~~~Havent DEFINED CONVERSER YET!!!~~~
   const FString converserDialogFolderPath = FString("/Game/RTS_Tutorial/Dialogs/") + FString(converser->GetGameName().ToString());

   // Check to see if valid directory
   IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
   if (!platformFile.DirectoryExists(*converserDialogFolderPath)) return;

   FString loadedString;

   // For each dialog topic currently available, load the appropriate conversations
   for (FGameplayTag name : basePlayerRef->GetDialogTopics()) {
      const FString converserDialogPath = converserDialogFolderPath + "/" + name.ToString();
      FFileHelper::LoadFileToString(loadedString, *converserDialogPath, FFileHelper::EHashOptions::ErrorMissingHash);

      dialogs.Emplace(name, MoveTemp(loadedString)); // moved objects still have a valid state, so its old member functions exhibit defined behavior
   }
}

FString UDialogComponent::MakeFilePath(FGameplayTag* topic)
{
   return FString();
}

/*
#define LOCTEXT_NAMESPACE "Dialog"

TArray<FDialogData> UDialogComponent::JSONToDialogNodes(FString jsonString)
{
        //Setup reused variables
        TArray<FDialogData> dialogNodes;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*jsonString);
        TSharedPtr<FJsonValue> parsedJson;
        FDialogData data;

        if (FJsonSerializer::Deserialize(Reader, parsedJson))
        {
                TArray<TSharedPtr<FJsonValue>> nodeArray = parsedJson.Get()->AsArray();
                for (TSharedPtr<FJsonValue> node : nodeArray)
                {
                        TSharedPtr<FJsonObject> obj = node->AsObject();
                        data.actorName = *obj.Get()->GetStringField("actor");
                        FFormatOrderedArguments args;
                        args.Add(FText::FromString(data.actorName.ToString()));
                        args.Add(FText::FromString(obj.Get()->GetStringField("text")));
                        data.dialogue = FText::Format(LOCTEXT("{0}", "{1}"), args);
                        for (TSharedPtr<FJsonValue> value : obj.Get()->GetArrayField("nextDialogue"))
                        {
                                data.nextDialogue.Add(value->AsNumber());
                        }
                        dialogNodes.Add(data);
                }
        }
        return dialogNodes;
}
#undef LOCTEXT_NAMESPACE
*/

void UDialogComponent::LoadDialog(FString filePath)
{
   FString loadedString;
   FFileHelper::LoadFileToString(loadedString, *filePath, FFileHelper::EHashOptions::ErrorMissingHash);
   // JSONToDialogNodes(loadedString);
}

void UDialogComponent::Talk(FGameplayTag topic)
{
   if (dialogs.Contains(topic)) {}
}

void UDialogComponent::UpdateDialogs(FGameplayTag newTopic)
{
   // if(!dialogs.Contains(newTopic))
   // dialogs.Add(newTopic,)
}
