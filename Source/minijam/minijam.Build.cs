// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class minijam : ModuleRules
{
	public minijam(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"minijam",
			"minijam/Variant_Platforming",
			"minijam/Variant_Platforming/Animation",
			"minijam/Variant_Combat",
			"minijam/Variant_Combat/AI",
			"minijam/Variant_Combat/Animation",
			"minijam/Variant_Combat/Gameplay",
			"minijam/Variant_Combat/Interfaces",
			"minijam/Variant_Combat/UI",
			"minijam/Variant_SideScrolling",
			"minijam/Variant_SideScrolling/AI",
			"minijam/Variant_SideScrolling/Gameplay",
			"minijam/Variant_SideScrolling/Interfaces",
			"minijam/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
