#include "UpVisualizationCommands.h"
#include "EditorStyleSet.h"
#include "SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "UpVisualizationCommands"

FUpVisualizationCommands::FUpVisualizationCommands() :
    TCommands<FUpVisualizationCommands>(TEXT("UpVisualizers"),                                                                 // Context name for fast lookup
                                        NSLOCTEXT("Contexts", "UpVisualizationCommandsDesc", "Commands to Visualize Editor "), // Localized context name for displaying
                                        NAME_None,
                                        FEditorStyle::GetStyleSetName()) // Icon Style Set)
{
}

void FUpVisualizationCommands::RegisterCommands()
{
   UI_COMMAND(TogglePatrolPathVisualizer, "Patrol Path", "See patrol component paths.", EUserInterfaceActionType::Button, FInputChord());
   UI_COMMAND(ToggleCameraBoundsVisualizer, "Camera Bounds", "See boundaries player can pan camera.", EUserInterfaceActionType::Button, FInputChord());
   UI_COMMAND(ToggleCameraInitLocationVisualizer, "Camera Start", "See starting place of camera.", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
