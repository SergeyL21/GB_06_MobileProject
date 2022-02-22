// Fill out your copyright notice in the Description page of Project Settings.

using System;
using System.IO;
using UnrealBuildTool;

public class MobileProject : ModuleRules
{
	public MobileProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "ImageWrapper"
		});
		
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "Launch", "ApplicationCore" });
			PrivateIncludePaths.AddRange(new string[] { "/Source/Runtime/Launch/Private" });
			string pluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
			AdditionalPropertiesForReceipt.Add(new ReceiptProperty(
				"TestInterface", 
				Path.Combine(pluginPath, "TestInterface_APL.xml")));

		}
	}
}
