// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class MobileProject : ModuleRules
{
	public MobileProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			string ThirdPartyLibsPath = Path.Combine(ModuleDirectory, "ThirdParty");
			
			PublicIncludePaths.AddRange(new string[]
			{
				Path.Combine(ThirdPartyLibsPath, "pugiXML", "src"),
				Path.Combine(ThirdPartyLibsPath, "MyTestLib", "src")
			});
			
			PublicAdditionalLibraries.AddRange(new string[]
			{
				Path.Combine(ThirdPartyLibsPath, "pugiXML", "pugixml.lib", "a"),
				Path.Combine(ThirdPartyLibsPath, "MyTestLib", "MyTestLib.lib", "a")
			});
		}
	}
}
