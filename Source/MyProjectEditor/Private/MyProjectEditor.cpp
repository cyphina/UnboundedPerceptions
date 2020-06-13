#include "MyProjectEditor.h"
#include "UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"
#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "FCustomTriggerDetailPanel.h"

/**Access appropriate module via module manager and call a registration function*/

DEFINE_LOG_CATEGORY(MyProjectEditor);

#define LOCTEXT_NAMESPACE "MyProjectEditor"

void FMyProjectEditorModule::StartupModule()
{
   UE_LOG(MyProjectEditor, Warning, TEXT("MyProjectEditor: Log Started"));
   FPropertyEditorModule& propertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
   propertyModule.RegisterCustomPropertyTypeLayout("TriggerData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FTriggerDataCustomization::MakeInstance));
   propertyModule.NotifyCustomizationModuleChanged();
}

void FMyProjectEditorModule::ShutdownModule()
{
   UE_LOG(MyProjectEditor, Warning, TEXT("MyProjectEditor: Log Ended"));
   if(FModuleManager::Get().IsModuleLoaded("PropetyEditor")) {
      FPropertyEditorModule* propertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor");
      propertyModule->UnregisterCustomClassLayout("FTriggerData");
   }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMyProjectEditorModule, MyProjectEditor)
