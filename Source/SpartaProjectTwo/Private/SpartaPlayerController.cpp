
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LampItem.h"

ASpartaPlayerController::ASpartaPlayerController()
	:InputMappingContext(nullptr), MoveAction(nullptr), JumpAction(nullptr), LookAction(nullptr), SprintAction(nullptr),
	HUDWidgetClass(nullptr), HUDWidgetInstance(nullptr), MainMenuWidgetClass(nullptr), MainMenuWidgetInstance(nullptr), StartMenuWidgetClass(nullptr), StartMenuWidgetInstance(nullptr)
{
	//테스트
	UE_LOG(LogTemp, Warning, TEXT("ASpartaPlayerController to reset"));
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp,Warning,TEXT("ControllerBeginPlay"))
	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0); // 0 : 우선순위
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowStartMenu();
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
			{
				int32 CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
				if (CurrentLevelIndex == 0) return;
				GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Level %d Start!"), CurrentLevelIndex));
			}
		}
	}
}

UUserWidget* ASpartaPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

//mainManu 보여주는 함수 ( GameOver도 )
void ASpartaPlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		//화면에서 위젯이 사라짐
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (StartMenuWidgetInstance)
	{
		StartMenuWidgetInstance->RemoveFromParent();
		StartMenuWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}



	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonBlock"))))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart")));
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("Start")));
			}
		}

		//GameOver
		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}
			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score : %d"), SpartaGameInstance->TotalScore)));
				}
			}
		}
	}
}

void ASpartaPlayerController::ShowStartMenu()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (StartMenuWidgetInstance)
	{
		StartMenuWidgetInstance->RemoveFromParent();
		StartMenuWidgetInstance = nullptr;
	}
	if (StartMenuWidgetClass)
	{
		StartMenuWidgetInstance = CreateWidget<UUserWidget>(this, StartMenuWidgetClass);
		if (StartMenuWidgetInstance)
		{
			StartMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void ASpartaPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}

		ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
		if (SpartaGameState)
		{
			SpartaGameState->FirstUpdateHUD();
			SpartaGameState->UpdateHUD();
		}
	}
}

void ASpartaPlayerController::StartGame()
{
	if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SpartaGameInstance->TotalScore = 0;
		SpartaGameInstance->CurrentLevelIndex = 0;
	}

	SetPause(false);
	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
}

void ASpartaPlayerController::GoMainMenu()
{
	if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SpartaGameInstance->TotalScore = 0;
		SpartaGameInstance->CurrentLevelIndex = 0;
	}

	SetPause(false);
	UGameplayStatics::OpenLevel(GetWorld(), FName("MenuLevel"));
}

void ASpartaPlayerController::EndGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr,EQuitPreference::Quit, false);
}
