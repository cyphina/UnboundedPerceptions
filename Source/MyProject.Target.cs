// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class MyProjectTarget : TargetRules
{
	public MyProjectTarget(TargetInfo targetInfo) : base(targetInfo)
	{
		Type = TargetType.Game;
	    ExtraModuleNames.AddRange(new string[] {
	        "MyProject"
	    });
    }
}
