// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TestProject : ModuleRules
{
	public TestProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "AIModule", "NavigationSystem", "AnimGraphRuntime", "EnhancedInput", "Niagara", "MediaAssets", "MoviePlayer" });

        PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });

        // Uncomment when using slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
