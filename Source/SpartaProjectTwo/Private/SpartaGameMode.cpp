// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"

ASpartaGameMode::ASpartaGameMode()
{
	// C++������ �����ϰ� �ѹ� �� ���ش�?? ��¥ ����
	DefaultPawnClass = ASpartaCharacter::StaticClass();//Static�̶� ��ü���� �� ���൵ �������ش� / UCLASS �������� Ŭ������ ��ȯ���ش� / Ŭ���� ������ �Ѱܼ� ������ �� ���� ����Ѵ�

	//�÷��̾� ��Ʈ�ѷ�
	PlayerControllerClass = ASpartaPlayerController::StaticClass();

	//GameState Ŭ����
	GameStateClass = ASpartaGameState::StaticClass();

}
