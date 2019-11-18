#include "MyProjectEditorVisualizations.h"
#include "WorldObjects/PatrolComponent.h"
#include "Visualizations/PatrolPoints.h"
#include "UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "UnrealEd.h"

/**Access appropriate module via module manager and call a registration function*/

DEFINE_LOG_CATEGORY(MyProjectEditorVisualizations);

#define LOCTEXT_NAMESPACE "MyProjectEditorVisualizations"

void FMyProjectEditorVisualizationsModule::StartupModule()
{
	UE_LOG(MyProjectEditorVisualizations, Warning, TEXT("MyProjectEditorVisualizations: Log Started"));
	if (GUnrealEd != NULL)
	{
	   GUnrealEd->RegisterComponentVisualizer(UPatrolComponent::StaticClass()->GetFName(),
						  MakeShareable(new FPatrolVisualizer));
	}
}

void FMyProjectEditorVisualizationsModule::ShutdownModule()
{
	if (GUnrealEd != NULL)
	{
		UE_LOG(MyProjectEditorVisualizations, Warning, TEXT("MyProjectEditorVisualizations: Log Ended"));
		GUnrealEd->UnregisterComponentVisualizer(UPatrolComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMyProjectEditorVisualizationsModule, MyProjectEditorVisualizations)
