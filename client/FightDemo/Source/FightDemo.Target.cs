

using UnrealBuildTool;
using System.Collections.Generic;

public class FightDemoTarget : TargetRules
{
	public FightDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "FightDemo" } );
	}
}
