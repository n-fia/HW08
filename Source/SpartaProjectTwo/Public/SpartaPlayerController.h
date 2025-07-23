// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpartaPlayerController.generated.h"

//IMC 미리 선언
class UInputMappingContext;
class UInputAction;

UCLASS()
class SPARTAPROJECTTWO_API ASpartaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//플레이어컨트롤러와 인풋매핑 선언
	ASpartaPlayerController(); //생성자

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass; // UUserWidget : UI전용 클래스 (UMG (Unreal Motion Graphics)에서 사용)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass; // UUserWidget : UI전용 클래스 (UMG (Unreal Motion Graphics)에서 사용)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StartMenu")
	TSubclassOf<UUserWidget> StartMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StartMenu")
	UUserWidget* StartMenuWidgetInstance;


	UFUNCTION(BlueprintCallable, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintCallable, Category="HUD")
	void ShowGameHUD();

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "StartMenu")
	void ShowStartMenu();

	UFUNCTION(BlueprintCallable, Category = "StartMenu")
	void GoMainMenu();

	UFUNCTION(BlueprintCallable, Category = "GameMenu")
	void EndGame();

protected:
	virtual void BeginPlay() override;
};
