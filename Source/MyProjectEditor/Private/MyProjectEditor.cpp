#include "MyProjectEditor.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"
#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "CustomDetails/FCustomTriggerDetailPanel.h"
#include "CustomDetails/InteractableDetails.h"
#include "LevelEditor.h"
#include "CustomDetails/UnitDetails.h"
#include "CustomDetails/UnitControllerDetails.h"
#include "UpEditorCommands.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Editor/Blutility/Classes/EditorUtilityWidgetBlueprint.h"
#include "DesignerUtilities/SRTSTriggerEditor.h"

/**Access appropriate module via module manager and call a registration function*/

DEFINE_LOG_CATEGORY(MyProjectEditor);

#define LOCTEXT_NAMESPACE "MyProjectEditor"

void FMyProjectEditorModule::StartupModule()
{
   UE_LOG(MyProjectEditor, Warning, TEXT("MyProjectEditor: Module Started"));

   IUpEditorModule::StartupModule();

   FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
   RegisterCustomClassLayouts(PropertyModule);
   RegisterCustomPropertyLayouts(PropertyModule);

   FUpCommonCommands::Register();
   WindowTabExtension();

   PropertyModule.NotifyCustomizationModuleChanged();
}

void FMyProjectEditorModule::ShutdownModule()
{
   UE_LOG(MyProjectEditor, Warning, TEXT("MyProjectEditor: Module Shutdown"));

   IUpEditorModule::ShutdownModule();

   if(FSlateApplication::IsInitialized())
   {
      FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName(TEXT("TriggerManager")));

      if(TriggerEditorTab.IsValid())
      {
         TriggerEditorTab.Pin()->RequestCloseTab();
      }
   }

   if(FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
   {
      FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor");
      PropertyModule->UnregisterCustomClassLayout("Unit");
      PropertyModule->UnregisterCustomClassLayout("UnitController");
      PropertyModule->UnregisterCustomClassLayout("InteractableBase");
      PropertyModule->UnregisterCustomPropertyTypeLayout("FTriggerData");
   }

   if(DesignToolsExt.IsValid() && FModuleManager::Get().IsModuleLoaded("LevelEditor"))
   {
      FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
      LevelEditorModule.GetMenuExtensibilityManager()->RemoveExtender(DesignToolsExt);
      FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TRIGGER_MANAGER_TAB_NAME);
   }

   FUpCommonCommands::Unregister();
}

void FMyProjectEditorModule::AddModuleListeners()
{
}

void FMyProjectEditorModule::WindowTabExtension()
{
   DesignToolsExt = MakeShareable(new FExtender);
   DesignToolsExt->AddMenuExtension("General", EExtensionHook::After, nullptr,
                                    FMenuExtensionDelegate::CreateRaw(this, &FMyProjectEditorModule::DesignerToolMenuExtension));
   FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
   LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(DesignToolsExt);

   FGlobalTabmanager::Get()
       ->RegisterNomadTabSpawner(TRIGGER_MANAGER_TAB_NAME, FOnSpawnTab::CreateRaw(this, &FMyProjectEditorModule::CreateTriggerManagerTab))
       .SetDisplayName(LOCTEXT("TriggerManager", "Trigger Manager"))
       .SetTooltipText(NSLOCTEXT("GameplayAbilitiesEditorModule", "TriggerManagerToolTip", "Open Trigger Manager tab."))
       .SetIcon(FSlateIcon(FUpEditorStyle::Get().GetStyleSetName(), FUpEditorStyle::Get().GetEditorTriggerManagerIconStyleName()));
}

void FMyProjectEditorModule::DesignerToolMenuExtension(FMenuBuilder& MenuBuilder)
{
   MenuBuilder.AddSubMenu(LOCTEXT("DesignerTools", "Designer Tools"), LOCTEXT("DesignerToolsToolTip", "Designer workflow tools"),
                          FMenuExtensionDelegate::CreateStatic(&FMyProjectEditorModule::DesignerToolSubMenuContents), false,
                          FSlateIcon(FUpEditorStyle::Get().GetStyleSetName(), FUpEditorStyle::Get().GetEditorDesignSubMenuIconStyleName()));
}

TSharedRef<SDockTab> FMyProjectEditorModule::CreateTriggerManagerTab(const FSpawnTabArgs& Args)
{
   return SAssignNew(TriggerEditorTab, SDockTab).TabRole(ETabRole::NomadTab)[CreateTriggerManagerSWidget().ToSharedRef()];
}

TSharedPtr<SWidget> FMyProjectEditorModule::CreateTriggerManagerSWidget()
{
   TSharedPtr<SRTSTriggerEditor> TriggerEditorPtr = SNew(SRTSTriggerEditor);
   TriggerEditor                                  = TriggerEditorPtr;
   return TriggerEditorPtr;
}

// Keep this above code where it is used.
auto FMyProjectEditorModule::GetEditorUtilityWidgetDelegate(const FName EditorWidgetPath)
{
   return FExecuteAction::CreateStatic(&FMyProjectEditorModule::LoadEditorUtilityWidget,
                                       FName("EditorUtilityWidgetBlueprint" + '\'' + EditorWidgetPath.ToString() + '\''));
}

void FMyProjectEditorModule::DesignerToolSubMenuContents(FMenuBuilder& MenuBuilder)
{
   TSharedPtr<FUICommandList> CommandList = MakeShareable(new FUICommandList());

   CommandList->MapAction(FUpCommonCommands::Get().OpenLevelTriggerList, GetEditorUtilityWidgetDelegate(FName(EDITOR_UTILITY_WIDGET_BP_PATH + "EW")));

   MenuBuilder.PushCommandList(CommandList.ToSharedRef());
   MenuBuilder.AddMenuEntry(FUpCommonCommands::Get().OpenLevelTriggerList, NAME_None, TAttribute<FText>(), TAttribute<FText>(),
                            GetUpEditorIcon(FUpEditorStyle::Get().GetEditorTriggerManagerIconStyleName()));
   MenuBuilder.PopCommandList();
   FGlobalTabmanager::Get()->PopulateTabSpawnerMenu(MenuBuilder, "TriggerManagerEW");
}

void FMyProjectEditorModule::LoadEditorUtilityWidget(FName AssetPath)
{
   FGlobalTabmanager::Get()->TryInvokeTab(FTabId(TRIGGER_MANAGER_TAB_NAME));
   return;

   FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
   // From 'Copy Reference' option.
   // TODO: Store the editor widget folder path as a variable and maybe move this logic outside of this class.
   const FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(AssetPath);
   if(AssetData.IsValid())
   {
      if(UEditorUtilityWidgetBlueprint* TriggerManangementEditorGenClass = Cast<UEditorUtilityWidgetBlueprint>(AssetData.GetAsset()))
      {
         const FName RegistrationName = FName(*(TriggerManangementEditorGenClass->GetPathName() + TEXT("_ActiveTab")));

         FLevelEditorModule&         LevelEditorModule     = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
         const TWeakPtr<FTabManager> LevelEditorTabManager = LevelEditorModule.GetLevelEditorTabManager();

         if(LevelEditorTabManager.IsValid())
         {
            TSharedPtr<FTabManager> TabManager = LevelEditorTabManager.Pin();
         }
      }
   }
}

FSlateIcon FMyProjectEditorModule::GetUpEditorIcon(FName IconName)
{
   return FSlateIcon(FUpEditorStyle::Get().GetStyleSetName(), IconName);
}

void FMyProjectEditorModule::RegisterCustomPropertyLayouts(FPropertyEditorModule& PropertyModule)
{
   PropertyModule.RegisterCustomPropertyTypeLayout("TriggerData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FTriggerDataCustomization::MakeInstance));
}

void FMyProjectEditorModule::RegisterCustomClassLayouts(FPropertyEditorModule& PropertyModule)
{
   PropertyModule.RegisterCustomClassLayout("Unit", FOnGetDetailCustomizationInstance::CreateStatic(&FUnitDetails::MakeInstance));
   PropertyModule.RegisterCustomClassLayout("UnitController", FOnGetDetailCustomizationInstance::CreateStatic(&FUnitControllerDetails::MakeInstance));
   PropertyModule.RegisterCustomClassLayout("InteractableBase", FOnGetDetailCustomizationInstance::CreateStatic(&FInteractableDetails::MakeInstance));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMyProjectEditorModule, MyProjectEditor)
