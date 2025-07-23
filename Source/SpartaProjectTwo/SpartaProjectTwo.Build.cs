// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpartaProjectTwo : ModuleRules
{
	public SpartaProjectTwo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //5개의 입력 + 내가 HUD 추가
        //UMG : Unreal Motion Graphics
        //필수적인 엔진 기능들
        //												엔진 주요 기능 / 리플렉션 시스템 / 게임 엔진의 주요기능 / 입력 시스템 / EnhancedInput관련 / 언리얼 위젯 관련 / UI 관련
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Slate", "SlateCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
