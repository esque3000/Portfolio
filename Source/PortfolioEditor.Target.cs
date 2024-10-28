// Copyright (c) 2024 Jiten Ukani

using UnrealBuildTool;
using System.Collections.Generic;

public class PortfolioEditorTarget : TargetRules
{
	public PortfolioEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("Portfolio");
	}
}
