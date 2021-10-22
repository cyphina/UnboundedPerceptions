#pragma once
#include "SUserWidget.h"

UENUM()
enum class ETriggerSortOption : uint8
{
   Ascending,
   Descending
};

class SRTSTriggerEditor : public SUserWidget
{
   SLATE_USER_ARGS(SRTSTriggerEditor) {}
   SLATE_END_ARGS()

   virtual void Construct(const FArguments& InArgs)                              = 0;
   virtual void HandleNotify_FindAssetInEditor(FString AssetName, int AssetType) = 0;
   virtual void HandleNotify_OpenAssetInEditor(FString AssetName, int AssetType) = 0;
};
