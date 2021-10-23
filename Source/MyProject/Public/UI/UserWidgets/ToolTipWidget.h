#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToolTipWidget.generated.h"

/**
 * Widget for tooltips
 */

class UTextBlock;

UCLASS()
class MYPROJECT_API UToolTipWidget : public UUserWidget
{
   GENERATED_BODY()

 protected:
   UPROPERTY(BlueprintReadWrite, Category = "Action", Meta = (BindWidget))
   UTextBlock* titleText;

   UPROPERTY(BlueprintReadWrite, Category = "Action", Meta = (BindWidget))
   UTextBlock* descText;

   UPROPERTY(BlueprintReadWrite, Category = "Action", Meta = (BindWidget))
   UTextBlock* moreInfoText;

   UPROPERTY(BlueprintReadWrite, Category = "Action", Meta = (BindWidget))
   UTextBlock* evenMoreInfoText;

   UPROPERTY(BlueprintReadWrite, Category = "Action", Meta = (BindWidget))
   UTextBlock* metaDataText;

 public:
   void SetTitleText(const FText& newText);
   void SetDescText(const FText& newText);
   void SetMoreInfoText(const FText& newText);
   void SetEvenMoreInfoText(const FText& newText);
   void SetMetaDataText(const FText& newText);

   /**tooltip box display -- ttText is description and ttText2 is another box for more situational information
   * @param name - Title block for the tooltip window
   * @param ttText - First block for information.  If any ttText's are empty strings, the corresponding blocks will be hidden
   * @param ttText2 - Second block for information
   * @param ttText3 - Third block for information
   * @param ttText4 - Fourth block for information
   */
   UFUNCTION(BlueprintCallable, Category = "Help")
   void SetupTTBoxText(const FText& name, const FText& ttText, const FText& ttText2, const FText& ttText3, const FText& ttText4);
};
