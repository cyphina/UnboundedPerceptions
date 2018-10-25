// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DialogComponent.generated.h"

/*
 * This component is for more detailed interactions with a WorldObject.
 */

class IWorldObject;
class ABasePlayer;
class Graph;
struct FDialogData;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UDialogComponent : public UActorComponent
{
   GENERATED_BODY()

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "Info")
   TMap<FGameplayTag, FString> dialogs;

   IWorldObject* converser;

   ABasePlayer* basePlayerRef;

   FString test;

   // Make a filepath to the dialog given the topic
   FString MakeFilePath(FGameplayTag* topic);
   // Convert the JSON array to an array of DialogData
   // TArray<FDialogData>					JSONToDialogNodes(FString jsonString);
   // Load up dialogue tree
   UFUNCTION(BlueprintCallable, Category = "File Handling")
   void LoadDialog(FString filePath);

 public:
   UDialogComponent();

   // Load up dialogue in beginplay.  Only loads how many that
   void BeginPlay() override;

   void Talk(FGameplayTag topic);
   // L
   void UpdateDialogs(FGameplayTag newTopic);
};
