// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MyProject : ModuleRules
{
	public MyProject(ReadOnlyTargetRules Target) : base(Target)
	{
        //MinFilesUsingPrecompiledHeaderOverride = 1;
       // bFasterWithoutUnity = true;
        bUseRTTI = true;                

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "GameplayTasks", "GameplayTags", "GameplayAbilities", "AIModule", "MyProjectEditor"});
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" ,"Json", "JsonUtilities", "GameplayAbilities", "AIModule"});
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		// {
		//		if (UEBuildConfiguration.bCompileSteamOSS == true)
		//		{
		//			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		//		}
		// }
	}
}
