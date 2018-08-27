// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "ItemExamineWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UItemExamineWidget : public UMyUserWidget
{
	GENERATED_BODY()

public:

	/**ID of the item who's picture should be examined when the item is used*/
	UPROPERTY(BlueprintReadWrite)
	int								itemToDisplayID;

	/**Maps item id which is specified when this hud is opened to a corresponding picture*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int, UTexture2D*>			itemIDToDetailedPicture;							
};
