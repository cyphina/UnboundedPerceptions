// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "SaveLoadMenu.h"

void USaveLoadMenu::GetSavedFileNamesAndTimes(TArray<FString>& fileNames, TArray<FDateTime>& fileCreationTimes)
{
   FString        saveFilesPath = FPaths::ProjectDir().Append("\\SavedGames\\");
   IPlatformFile& platformFile  = FPlatformFileManager::Get().GetPlatformFile();
   if (!platformFile.DirectoryExists(*saveFilesPath)) {
      platformFile.CreateDirectory(*saveFilesPath);
      return;
   }

   fileNames.Empty();
   fileCreationTimes.Empty();
   platformFile.FindFiles(fileNames, *saveFilesPath, *FString(""));

   int i = 0;
   for (FString fileName : fileNames) {
      fileNames[i] = fileName.RightChop(saveFilesPath.Len());
      fileCreationTimes.Emplace(platformFile.GetTimeStamp(*fileName));
      ++i;
   }
}
