#pragma once

#include "Framework/Commands/Commands.h"
#include "SlateStyle.h"

/** Based off EditorViewportCommands.h */
class FUpVisualizationCommands : public TCommands<FUpVisualizationCommands>
{
 public:
   FUpVisualizationCommands();

   /** Initialize commands */
   virtual void RegisterCommands() override;

   TSharedPtr<FUICommandInfo> TogglePatrolPathVisualizer;

   TSharedPtr<FUICommandInfo> ToggleCameraBoundsVisualizer;

   TSharedPtr<FUICommandInfo> ToggleCameraInitLocationVisualizer;
};
