#pragma once

#include "Blueprint/UserWidget.h"
#include "UI/UserWidgetExtensions/RTSBrowserWidget.h"
#include "RTSBrowserWidgetBase.generated.h"

UCLASS()
class MYPROJECT_API URTSBrowserWidgetBase : public UUserWidget, public IRTSBrowserWidget
{
   GENERATED_BODY()

 public:
   UFUNCTION(BlueprintCallable, BlueprintPure)
   FORCEINLINE UBluEye* GetBrowser() const { return browser; }

   /** URL of the site the browser loaded*/
   UPROPERTY(EditDefaultsOnly)
   FString URL;

   UFUNCTION(BlueprintCallable)
   void StartDisplay(int browserWidth, int browserHeight) override final;

   /** Send JSON information to the browser to update UI*/
   void UpdateInformation(const FString& updateFunctionName, FStringView updateJSON) override final;

 protected:
   void NativeConstruct() override;

   /** Object interface to web browser*/
   UPROPERTY()
   UBluEye* browser;

   /** Material that the browser uses to render the browser to a texture*/
   UPROPERTY(EditDefaultsOnly)
   UMaterialInterface* renderMaterial;

   /** Texture the browser is rendered to*/
   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
   class UImage* browserCanvas;
};