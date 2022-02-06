// Fill out your copyright notice in the Description page of Project Settings.

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
			PublicAdditionalLibraries.Add(ModuleDirectory + "ThirdParty/pugiXML/pugixml.lib");
			/*PublicAdditionalLibraries.AddRange(new string[]
			{
				System.IO.Path.Combine(ModuleDirectory, "ThirdParty", "pugiXML", "pugixml.lib")
			});*/
		}
	}
}
