#pragma once

#include "SlateIcon.h"
#include "UpEditorModuleTypes.h"

class FPropertyEditorModule;
class FSpawnTabArgs;
class SDockTab;
class FMenuBuilder;
class FExtender;
class SRTSTriggerEditor;
class SWidget;

DECLARE_LOG_CATEGORY_EXTERN(MyProjectEditor, All, All);

class FMyProjectEditorModule : public IUpEditorModule
{
 public:
   virtual void StartupModule() override;
   virtual void ShutdownModule() override;

   virtual void AddModuleListeners() override;

   static FMyProjectEditorModule& Get() { return FModuleManager::LoadModuleChecked<FMyProjectEditorModule>(MODULE_NAME); }
   static bool                    IsAvailable() { return FModuleManager::Get().IsModuleLoaded(MODULE_NAME); }

   static const inline FString          EDITOR_UTILITY_WIDGET_BP_PATH = "/Game/RTS_Tutorial/HUDs/EditorWidgets/";
   static const inline FName            TRIGGER_MANAGER_TAB_NAME       = "TriggerManagerTab";
   static constexpr inline const TCHAR* MODULE_NAME                    = TEXT("MyProjectEditorModule");

 protected:
   void WindowTabExtension();

   /** Creates a designe tool menu inside Window tab */
   void        DesignerToolMenuExtension(FMenuBuilder& MenuBuilder);
   static void DesignerToolSubMenuContents(FMenuBuilder& MenuBuilder);

   /** Creates a new tab with a Trigger Manager widget inside it*/
   TSharedRef<SDockTab> CreateTriggerManagerTab(const FSpawnTabArgs& Args);

   /** Creates a Trigger Manager widget */
   TSharedPtr<SWidget> CreateTriggerManagerSWidget();

 private:
   /**
	 * @brief Change detail view of individual properties.
	 */
   void RegisterCustomPropertyLayouts(FPropertyEditorModule& PropertyModule);

   /**
	 * @brief Change detail views of several classes
	 * @param AssetPath - Path without BP type prefix
	 */
   void RegisterCustomClassLayouts(FPropertyEditorModule& PropertyModule);

   static void LoadEditorUtilityWidget(const FName AssetPath);

   static auto       GetEditorUtilityWidgetDelegate(const FName EditorWidgetPath);
   static FSlateIcon GetUpEditorIcon(FName IconName);

   TWeakPtr<SRTSTriggerEditor> TriggerEditor;
   TWeakPtr<SDockTab>          TriggerEditorTab;
   TSharedPtr<FExtender>       DesignToolsExt;
};
