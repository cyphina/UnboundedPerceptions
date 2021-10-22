#include "UpEditorCommands.h"
#include "EditorStyleSet.h"
#include "SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "UpEditorCommands"

FUpEditorStyle::FUpEditorStyle() : FSlateStyleSet("UpEditorStyle")
{
   const auto IconPath = FPaths::ProjectContentDir() / TEXT("Slate");
   FSlateStyleSet::SetContentRoot(IconPath);
   Set(GetEditorTriggerManagerIconStyleName(), new FSlateImageBrush(FSlateStyleSet::RootToContentDir(TEXT("hbhugsmall.png")), FVector2D(16.f, 16.f)));
   Set(GetEditorDesignSubMenuIconStyleName(), new FSlateImageBrush(FSlateStyleSet::RootToContentDir(TEXT("designtools.png")), FVector2D(16.f, 16.f)));
   Set(GetEditorUpVisIconStyleName(), new FSlateImageBrush(FSlateStyleSet::RootToContentDir(TEXT("eye.png")), FVector2D(16.f, 16.f)));

   FSlateStyleRegistry::RegisterSlateStyle(*this);
}

FUpEditorStyle::~FUpEditorStyle()
{
   FSlateStyleRegistry::UnRegisterSlateStyle(*this);
}

TOptional<FUpEditorStyle> FUpEditorStyle::Singleton;

FUpCommonCommands::FUpCommonCommands() :
    TCommands<FUpCommonCommands>(TEXT("UpEditorCommands"), NSLOCTEXT("Contexts", "UpEditorCommandContext", "Commands to initiate Up specific workflows"), NAME_None,
                                 FUpEditorStyle::Get().GetStyleSetName())
{
}

void FUpCommonCommands::RegisterCommands()
{
   UI_COMMAND(OpenLevelTriggerList, "Open Trigger Manager", "View List of All Triggers in Level.", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
