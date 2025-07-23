// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"

ASpartaGameMode::ASpartaGameMode()
{
	// C++에서도 안전하게 한번 더 해준다?? 진짜 안전
	DefaultPawnClass = ASpartaCharacter::StaticClass();//Static이라서 객체생성 안 해줘도 선언해준다 / UCLASS 형식으로 클래스를 반환해준다 / 클래스 정보를 넘겨서 관리할 때 자주 사용한다

	//플레이어 컨트롤러
	PlayerControllerClass = ASpartaPlayerController::StaticClass();

	//GameState 클래스
	GameStateClass = ASpartaGameState::StaticClass();

}
