#include "MyProjectEditor.h"

/**Access appropriate module via module manager and call a registration function*/

DEFINE_LOG_CATEGORY(MyProjectEditor);

#define LOCTEXT_NAMESPACE "MyProjectEditor"

void FMyProjectEditorModule::StartupModule()
{
	UE_LOG(MyProjectEditor, Warning, TEXT("MyProjectEditor: Log Started"));
}

void FMyProjectEditorModule::ShutdownModule()
{
	UE_LOG(MyProjectEditor, Warning, TEXT("MyProjectEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMyProjectEditorModule, MyProjectEditor)