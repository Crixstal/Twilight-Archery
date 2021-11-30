// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TwilightArchery : ModuleRules
{
	public TwilightArchery(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara" });

		//PublicIncludePaths.AddRange(new string[] { Target.Combine(ModuleDirectory, "Niagara") });
	}
}
