// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MyProjectEditor : ModuleRules
{
	public MyProjectEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		bEnforceIWYU = true;
		PrivatePCHHeaderFile = "Public/MyProjectEditor.h";
		CppStandard = CppStandardVersion.Cpp17;

		PublicIncludePaths.AddRange(new string[] { "MyProjectEditor/Public" });
		PrivateIncludePaths.AddRange(new string[] { "MyProjectEditor/Private" });
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"BlueprintGraph"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]{
			"MyProject",
			"BlueprintGraph"
		});
		
		if(Target.Type == TargetRules.TargetType.Editor) {
		PrivateDependencyModuleNames.AddRange(new string[]{		
			"Core",
			"InputCore",
			"Engine",
			"CoreUObject",
			"UnrealEd",		
			"GraphEditor",
			"PropertyEditor",
			"SlateCore",
			"Slate",
			"EditorStyle",
			"Kismet",
			"KismetCompiler",
			"GameplayAbilities",
			"GameplayTags",
			"Blutility",
			"UMGEditor",
			"EditorScriptingUtilities"
		});
		}
	}
}

