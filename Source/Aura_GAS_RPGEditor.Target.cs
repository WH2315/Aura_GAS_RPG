using UnrealBuildTool;
using System.Collections.Generic;

public class Aura_GAS_RPGEditorTarget : TargetRules
{
	public Aura_GAS_RPGEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V7;

		ExtraModuleNames.AddRange( new string[] { "Aura_GAS_RPG" } );
	}
}
