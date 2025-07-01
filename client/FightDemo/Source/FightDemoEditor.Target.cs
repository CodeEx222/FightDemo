

using UnrealBuildTool;
using System.Collections.Generic;

public class FightDemoEditorTarget : TargetRules
{
	public FightDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "FightDemo" } );
	}
}
