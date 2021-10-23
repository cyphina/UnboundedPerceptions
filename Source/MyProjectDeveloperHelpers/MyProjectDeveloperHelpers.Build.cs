// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MyProjectDeveloperHelpers : ModuleRules
{
	public MyProjectDeveloperHelpers(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.AddRange(new string[] { "MyProjectDeveloperHelpers/Public" });
		PrivateIncludePaths.AddRange(new string[] { "MyProjectDeveloperHelpers/Private" });
		PublicDependencyModuleNames.AddRange(new string[] { });
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

