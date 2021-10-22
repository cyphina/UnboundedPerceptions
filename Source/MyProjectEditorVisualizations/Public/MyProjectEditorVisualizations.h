#pragma once

#include "LevelEditor.h"
#include "Modules/ModuleManager.h"
#include "MultiBoxExtender.h"

class SCheckBox;
class FLevelEditorModule;
using LevelEditorExtender = FLevelEditorModule::FLevelEditorMenuExtender;
class FUICommandList;
class FExtensibilityManager;
class FExtender;
class FWorkspaceItem;
class UToolMenu;

DECLARE_LOG_CATEGORY_EXTERN(MyProjectEditorVisualizations, All, All);

/** Based on SLevelViewportToolBar.cpp */
class FMyProjectEditorVisualizations : public IModuleInterface
{
 public:
   virtual void StartupModule() override;
   virtual void ShutdownModule() override;

   static TSharedRef<FWorkspaceItem> GetMenuRoot() { return MenuRoot; }

 protected:
   void CreateViewOptionExtension();

 private:
   void GenerateUpVisMenuExtension(FMenuBuilder& MenuBuilder);
   void GenerateUpVisSubmenu(FMenuBuilder& MenuBuilder);

   void DrawPatrolPoints(ECheckBoxState bChecked);
   void DrawCameraPosition(ECheckBoxState bChecked);
   void DrawCameraBounds(ECheckBoxState bChecked);

   void TogglePatrolComponent();

   TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensionPoint;
   TSharedPtr<FExtender>             ViewOptionMenuExtender;
   static TSharedRef<FWorkspaceItem> MenuRoot;

   bool bTogCameraBoundsVis = false;
   bool bTogCameraPosVis    = false;
   bool bTogPatrolPointsVis = true;

   TSharedPtr<SWidget> TogCameraBoundsMenuWidget;
   TSharedPtr<SWidget> TogCameraPositionMenuWidget;
   TSharedPtr<SWidget> TogPatrolCompVisWidget;
};
