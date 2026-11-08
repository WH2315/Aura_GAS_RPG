using UnrealBuildTool;
using System.Collections.Generic;

public class Aura_GAS_RPGTarget : TargetRules
{
	public Aura_GAS_RPGTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V7;

		ExtraModuleNames.AddRange( new string[] { "Aura_GAS_RPG" } );
	}
}
