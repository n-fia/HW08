// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameState.h"
#include "SpawnVolume.h"
#include "SpartaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "CoinItem.h"
#include "SpartaGameInstance.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h" // UTextBlock 사용가능
#include "Components/ProgressBar.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 10.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
	FirstUpdateHUD();
	GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle, this, &ASpartaGameState::UpdateHUD,0.1f,true); //true : 반복
}

int32 ASpartaGameState::GetScore() const
{
	return Score; //각 레벨 별 스코어
}

//Coin 획득 후 로그
void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if(USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
	//Score += Amount;
}

void ASpartaGameState::StartLevel()
{
	//if (CurrentLevelIndex == 0) return;
	// 종료 후 재시작
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaplayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaplayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
			GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Level %d Start!"), CurrentLevelIndex+1));
		}
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	//월드에 스폰된 FoundVolumes 액터를 찾는
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;


	for (int32 i = 0; i < ItemToSpawn; ++i)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{	//아이템 랜덤 생성
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				//코인이면 스폰코인++
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	//30초지나면 OnLevelTimeUp()
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnLevelTimeUp, //시간 끝나면 처리 함수
		LevelDuration, //몇 초 지속
		false //반복
	);
}

//
void ASpartaGameState::OnLevelTimeUp()
{
	EndLevel();
}

//코인 먹으면 호출 in CoinItem.cpp
void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Your Coin Is %d / %d"), CollectedCoinCount, SpawnedCoinCount);

	//다 주우면 레벨 종료
	if (SpawnedCoinCount>0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}

//StartLevel에서 조건식으로 호출
void ASpartaGameState::EndLevel()
{
	//타이머 초기화
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	//인스턴스(영구적 변수 카운트)에 현재 레벨 적용
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			AddScore(Score);
			// 다음 레벨 인덱스로
			CurrentLevelIndex++;
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex > MaxLevels)
	{
		OnGameOver();
		return;
	}
	
	if(LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
		StartLevel();
	}
	else
	{
		//todo : 레벨 3까지 완료 후 엔딩
		OnGameOver();
	}
}

void ASpartaGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaplayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaplayerController->SetPause(true);
			SpartaplayerController->ShowMainMenu(true);
		}
	}
}

void ASpartaGameState::UpdateHUD()
{
	if (APlayerController * PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController);
		if (SpartaPlayerController)
		{
			UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget();
			if (HUDWidget)
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UProgressBar* ProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("TimeProgressBar"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					float TimeRate = RemainingTime/LevelDuration;
					ProgressBar->SetPercent(TimeRate);
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score : %d"), SpartaGameInstance->TotalScore)));
						}
					}
				}
			}
		}
	}
}

void ASpartaGameState::FirstUpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController);
		if (SpartaPlayerController)
		{
			UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget();
			if (HUDWidget)
			{
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("LEVEL : %d"), CurrentLevelIndex + 1)));
				}
				//test
				//if (CurrentLevelIndex==2)
				if (CurrentLevelIndex<3)
				{
					if (UTextBlock* StateText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("StateText"))))
					{
						//if (KRFont.FontObject)
						//{
						//	UE_LOG(LogTemp,Warning,TEXT("SetFont is Completed"))
						//	StateText->SetFont(KRFont);
						//}
						StateText->SetText(FText::FromString(FString::Printf(TEXT("과제과제 SetFont is 한글 확인용 Completed"))));
	/*					if (KRFont.TypefaceFontName.IsValid())
						{
							UE_LOG(LogTemp,Warning,TEXT("SetFont is Completed is %s"), *KRFont.TypefaceFontName.ToString())
							StateText->SetFont(KRFont);
						}*/
						FTimerHandle DestroyTimerHandle;
						TWeakObjectPtr<UTextBlock>  WeakTextPtr = StateText;

						GetWorld()->GetTimerManager().SetTimer(
							DestroyTimerHandle,
							[WeakTextPtr]()
							{
								if (WeakTextPtr.IsValid()) {
									WeakTextPtr->SetText(FText::FromString(FString::Printf(TEXT(""))));
									//UE_LOG(LogTemp, Warning, TEXT("Font is %s"), WeakTextPtr->GetFont().TypefaceFontName.ToString())
								}
							},
							3.0f, false
						);
					}
				}
			}
		}
	}
}