// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MyProjectEditorVisualizations : ModuleRules
{
	public MyProjectEditorVisualizations(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicIncludePaths.AddRange(new string[] {"MyProjectEditorVisualizations/Public"});
		PrivateIncludePaths.AddRange(new string[] {"MyProjectEditorVisualizations/Private"});
		PublicDependencyModuleNames.AddRange(new string[]{});
		PrivateDependencyModuleNames.AddRange(new string[]{
			"MyProject",
			"Core",
			"InputCore",
            "Engine",
            "CoreUObject",
            "UnrealEd",
            "BlueprintGraph",
            "GraphEditor",
            "PropertyEditor",
            "SlateCore",
            "Slate",
            "EditorStyle",
            "Kismet",
            "KismetCompiler",
		    "GameplayAbilities",
			"GameplayTags"
		});
	}
}

