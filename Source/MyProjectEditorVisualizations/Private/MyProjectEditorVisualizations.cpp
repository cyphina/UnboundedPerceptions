#include "MyProjectEditorVisualizations.h"
#include "AIStuff/AIControllers/Components/PatrolComponent.h"
#include "Visualizations/PatrolPoints.h"
#include "UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "UnrealEd.h"
#include "ToolMenus.h"
#include "UpVisualizationCommands.h"
#include "UPLevelScript.h"
#include "DrawDebugHelpers.h"
#include "IAssetViewport.h"
#include "SlateWrapperTypes.h"

/** Access appropriate module via module manager and call a registration function */

DEFINE_LOG_CATEGORY(MyProjectEditorVisualizations);

#define LOCTEXT_NAMESPACE "MyProjectEditorVisualizations"

void FMyProjectEditorVisualizations::StartupModule()
{
   UE_LOG(MyProjectEditorVisualizations, Display, TEXT("MyProjectEditorVisualizations: Log Started"));
   if(GUnrealEd != nullptr)
   {
      GUnrealEd->RegisterComponentVisualizer(UPatrolComponent::StaticClass()->GetFName(), MakeShareable(new FPatrolVisualizer));
      FUpVisualizationCommands::Register();
      CreateViewOptionExtension();
   }
}

void FMyProjectEditorVisualizations::ShutdownModule()
{
   UE_LOG(MyProjectEditorVisualizations, Display, TEXT("MyProjectEditorVisualizations: Log Ended"));
   if(GUnrealEd != nullptr)
   {
      GUnrealEd->UnregisterComponentVisualizer(UPatrolComponent::StaticClass()->GetFName());

      if(ViewOptionMenuExtender.IsValid() && FModuleManager::Get().IsModuleLoaded("LevelEditor"))
      {
         FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
         LevelEditorModule.GetMenuExtensibilityManager()->RemoveExtender(ViewOptionMenuExtender);
      }
   }
}

void FMyProjectEditorVisualizations::CreateViewOptionExtension()
{
   FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
   LevelEditorMenuExtensionPoint         = LevelEditorModule.GetMenuExtensibilityManager();

   ViewOptionMenuExtender = MakeShareable<FExtender>(new FExtender());
   ViewOptionMenuExtender->AddMenuExtension("ViewMode", EExtensionHook::Position::After, nullptr,
                                            FMenuExtensionDelegate::CreateRaw(this, &FMyProjectEditorVisualizations::GenerateUpVisMenuExtension));
   LevelEditorMenuExtensionPoint.Get()->AddExtender(ViewOptionMenuExtender);
}

void FMyProjectEditorVisualizations::GenerateUpVisMenuExtension(FMenuBuilder& MenuBuilder)
{
   MenuBuilder.AddMenuSeparator();
   MenuBuilder.AddSubMenu(LOCTEXT("UpVisualizerMenuExtension", "Up Visualizers"),
                          LOCTEXT("UpVisualizerMenuTT", "Different visuals displayed in the feature to help visualize game properties"),
                          FNewMenuDelegate::CreateRaw(this, &FMyProjectEditorVisualizations::GenerateUpVisSubmenu), false,
                          FSlateIcon("UpEditorStyle", "UpEditor.UpVisIcon"));
}

void FMyProjectEditorVisualizations::GenerateUpVisSubmenu(FMenuBuilder& MenuBuilder)
{
   const TSharedPtr<FUICommandList> CommandList = MakeShareable(new FUICommandList());
   CommandList->MapAction(
       FUpVisualizationCommands::Get().TogglePatrolPathVisualizer,
       FExecuteAction::CreateRaw(this, &FMyProjectEditorVisualizations::DrawPatrolPoints, bTogPatrolPointsVis ? ECheckBoxState::Checked : ECheckBoxState::Unchecked));

   CommandList->MapAction(
       FUpVisualizationCommands::Get().ToggleCameraInitLocationVisualizer,
       FExecuteAction::CreateRaw(this, &FMyProjectEditorVisualizations::DrawCameraPosition, bTogCameraPosVis ? ECheckBoxState::Checked : ECheckBoxState::Unchecked));

   CommandList->MapAction(
       FUpVisualizationCommands::Get().ToggleCameraBoundsVisualizer,
       FExecuteAction::CreateRaw(this, &FMyProjectEditorVisualizations::DrawCameraBounds, bTogCameraBoundsVis ? ECheckBoxState::Checked : ECheckBoxState::Unchecked));

   MenuBuilder.PushCommandList(CommandList.ToSharedRef());

   MenuBuilder.BeginSection(NAME_None, FText::FromString("Up Visualizers"));

   TSharedPtr<SCheckBox> CurrentMenuWidgetCheckbox;
   TogPatrolCompVisWidget =
       SNew(SHorizontalBox) +
       SHorizontalBox::Slot()[SAssignNew(CurrentMenuWidgetCheckbox, SCheckBox).OnCheckStateChanged_Raw(this, &FMyProjectEditorVisualizations::DrawPatrolPoints)]
           .AutoWidth() +
       SHorizontalBox::Slot()[SNew(STextBlock).Text(FText::FromString("PatrolPointVis"))].Padding(5, 0, 10, 0).FillWidth(1.0).VAlign(VAlign_Center);

   CurrentMenuWidgetCheckbox->SetIsChecked(bTogPatrolPointsVis ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

   const FUIAction* Action = CommandList->GetActionForCommand(FUpVisualizationCommands::Get().TogglePatrolPathVisualizer);
   MenuBuilder.AddMenuEntry(*Action, TogPatrolCompVisWidget.ToSharedRef(), TEXT("Tog Patrol Vis"),
                            FText::FromString("When selecting a patrol component, shows the points the unit will "), EUserInterfaceActionType::Check);

   TogCameraPositionMenuWidget =
       SNew(SHorizontalBox) +
       SHorizontalBox::Slot()[SAssignNew(CurrentMenuWidgetCheckbox, SCheckBox).OnCheckStateChanged_Raw(this, &FMyProjectEditorVisualizations::DrawCameraPosition)]
           .AutoWidth() +
       SHorizontalBox::Slot()[SNew(STextBlock).Text(FText::FromString("CameraStartPosVis"))].Padding(5, 0, 10, 0).FillWidth(1.0).VAlign(VAlign_Center);

   CurrentMenuWidgetCheckbox->SetIsChecked(bTogCameraPosVis ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

   Action = CommandList->GetActionForCommand(FUpVisualizationCommands::Get().ToggleCameraInitLocationVisualizer);
   MenuBuilder.AddMenuEntry(*Action, TogCameraPositionMenuWidget.ToSharedRef(), TEXT("Tog Camera Start Vis"),
                            FText::FromString("Shows a widget and line towards the camera spawn point"), EUserInterfaceActionType::Check);

   TogCameraBoundsMenuWidget =
       SNew(SHorizontalBox) +
       SHorizontalBox::Slot()[SAssignNew(CurrentMenuWidgetCheckbox, SCheckBox).OnCheckStateChanged_Raw(this, &FMyProjectEditorVisualizations::DrawCameraBounds)]
           .AutoWidth() +
       SHorizontalBox::Slot()[SNew(STextBlock).Text(FText::FromString("CameraBoundsVis"))].Padding(5, 0, 10, 0).FillWidth(1.0).VAlign(VAlign_Center);

   CurrentMenuWidgetCheckbox->SetIsChecked(bTogCameraBoundsVis ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

   Action = CommandList->GetActionForCommand(FUpVisualizationCommands::Get().ToggleCameraBoundsVisualizer);
   MenuBuilder.AddMenuEntry(*Action, TogCameraBoundsMenuWidget.ToSharedRef(), TEXT("Tog Camera Bounds Vis"),
                            FText::FromString("Toggle Visibility of Bounds User can Move Camera in Level"), EUserInterfaceActionType::Check);

   MenuBuilder.EndSection();
   MenuBuilder.PopCommandList();
}

void FMyProjectEditorVisualizations::DrawPatrolPoints(ECheckBoxState bChecked)
{
   if(GUnrealEd)
   {
      bTogPatrolPointsVis = !bTogPatrolPointsVis;
      if(bChecked == ECheckBoxState::Checked)
      {
         GUnrealEd->RegisterComponentVisualizer(UPatrolComponent::StaticClass()->GetFName(), MakeShareable(new FPatrolVisualizer));
      }
      else
      {
         GUnrealEd->UnregisterComponentVisualizer(UPatrolComponent::StaticClass()->GetFName());
         if(const UWorld* EditorWorld = GEditor->GetEditorWorldContext().World())
         {
            FlushPersistentDebugLines(EditorWorld);
         }
      }
      FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
      LevelEditorModule.GetFirstActiveViewport()->GetAssetViewportClient().Invalidate();
   }
}

void FMyProjectEditorVisualizations::DrawCameraPosition(ECheckBoxState bChecked)
{
   if(const UWorld* EditorWorld = GEditor->GetEditorWorldContext().World())
   {
      bTogCameraPosVis = !bTogCameraPosVis;
      if(bChecked == ECheckBoxState::Checked)
      {
         if(const AUPLevelScript* LevelScript = Cast<AUPLevelScript>(EditorWorld->GetLevelScriptActor()))
         {
            const FVector CameraStartLocation = LevelScript->GetCameraStartLocation();
            if(EditorWorld->ViewLocationsRenderedLastFrame.Num() > 0)
            {
               // Can also do this via GetLevelViewportCameraInfo()
               const FVector EditorCameraLocation = EditorWorld->ViewLocationsRenderedLastFrame[0];
               DrawDebugLine(EditorWorld, EditorCameraLocation, CameraStartLocation, FColor::Red, true, 100.f, 0, 10);
            }
            DrawDebugCamera(EditorWorld, CameraStartLocation, FRotator::ZeroRotator, 90, 1, FColor::Red, true, 100.f);
         }
      }
      else
      {
         FlushPersistentDebugLines(EditorWorld);
      }
      FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
      LevelEditorModule.GetFirstActiveViewport()->GetAssetViewportClient().Invalidate();
   }
}

void FMyProjectEditorVisualizations::DrawCameraBounds(ECheckBoxState bChecked)
{
   if(const UWorld* EditorWorld = GEditor->GetEditorWorldContext().World())
   {
      bTogCameraBoundsVis = !bTogCameraBoundsVis;
      if(bChecked == ECheckBoxState::Checked)
      {
         if(const AUPLevelScript* LevelScript = Cast<AUPLevelScript>(EditorWorld->GetLevelScriptActor()))
         {
            const auto CameraBoundaryPoints = LevelScript->GetCameraBounds();
            FBox       b;
            b += FVector(-CameraBoundaryPoints.Key.X, 0, 0);
            b += FVector(CameraBoundaryPoints.Key.Y, 0, 0);
            b += FVector(0, -CameraBoundaryPoints.Value.X, 0);
            b += FVector(0, CameraBoundaryPoints.Value.Y, 0);
            b += FVector(0, 0, -1000);
            b += FVector(0, 0, 1000);
            DrawDebugBox(EditorWorld, b.GetCenter(), b.GetExtent(), FColor::Red, true, 0, 100.f);
         }
      }
      else
      {
         FlushPersistentDebugLines(EditorWorld);
      }
      FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
      LevelEditorModule.GetFirstActiveViewport()->GetAssetViewportClient().Invalidate();
   }
}

void FMyProjectEditorVisualizations::TogglePatrolComponent()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMyProjectEditorVisualizations, MyProjectEditorVisualizations)
