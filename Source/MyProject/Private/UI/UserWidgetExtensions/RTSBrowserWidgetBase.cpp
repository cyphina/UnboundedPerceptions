#include "UI/UserWidgetExtensions/RTSBrowserWidgetBase.h"
#include "BluEye.h"
#include "BluBlueprintFunctionLibrary.h"
#include "Image.h"

void URTSBrowserWidgetBase::NativeConstruct()
{
   Super::NativeConstruct();
}

void URTSBrowserWidgetBase::StartDisplay(int browserWidth, int browserHeight)
{
   browser                 = UBluBlueprintFunctionLibrary::NewBluEye(this);
   browser->DefaultURL     = URL;
   browser->Width          = browserWidth;
   browser->Height         = browserHeight;
   browser->BaseMaterial   = renderMaterial;
   browser->bEnabled       = true;
   browser->bIsTransparent = true;
   browser->bEnableWebGL   = false;
   browser->init();
   browserCanvas->SetBrushFromTexture(browser->GetTexture());
}

void URTSBrowserWidgetBase::UpdateInformation(const FString& updateFunctionName, const FString& updateJSON)
{
   TArray<FString> parameters;
   parameters.Emplace(updateJSON);
   GetBrowser()->ExecuteJSMethodWithParams(updateFunctionName, parameters);
}
