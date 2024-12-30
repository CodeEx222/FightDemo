

using UnrealBuildTool;
using System.Collections.Generic;

public class FightDemoEditorTarget : TargetRules
{
	public FightDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "FightDemo" } );
	}
}
