// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GraveyardShiftEditorTarget : TargetRules
{
	public GraveyardShiftEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		ExtraModuleNames.Add("GraveyardShift");	
		ExtraModuleNames.Add("Test");
		ExtraModuleNames.Add("BaseFirstPersonCharacter");
	}
}
