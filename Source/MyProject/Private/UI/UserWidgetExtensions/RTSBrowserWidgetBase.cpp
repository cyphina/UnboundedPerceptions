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
   browser->Settings.Width          = browserWidth;
   browser->Settings.Height         = browserHeight;
   browser->BaseMaterial   = renderMaterial;
   browser->bEnabled       = true;
   browser->Settings.bIsTransparent = true;
   browser->Settings.bEnableWebGL   = false;
   browser->Init();
   browserCanvas->SetBrushFromTexture(browser->GetTexture());
}

void URTSBrowserWidgetBase::UpdateInformation(const FString& updateFunctionName, FStringView updateJSON)
{
   TArray<FString> parameters;
   parameters.Emplace(updateJSON);
   GetBrowser()->ExecuteJSMethodWithParams(updateFunctionName, parameters);
}
