// Copyright (c) 2024 Jiten Ukani

using UnrealBuildTool;
using System.Collections.Generic;

public class PortfolioTarget : TargetRules
{
	public PortfolioTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("Portfolio");
	}
}
