// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameState.h"
#include "SpawnVolume.h"
#include "SpartaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "CoinItem.h"
#include "SpartaGameInstance.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h" // UTextBlock ��밡��
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
	GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle, this, &ASpartaGameState::UpdateHUD,0.1f,true); //true : �ݺ�
}

int32 ASpartaGameState::GetScore() const
{
	return Score; //�� ���� �� ���ھ�
}

//Coin ȹ�� �� �α�
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
	// ���� �� �����
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
	//���忡 ������ FoundVolumes ���͸� ã��
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;


	for (int32 i = 0; i < ItemToSpawn; ++i)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{	//������ ���� ����
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				//�����̸� ��������++
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	//30�������� OnLevelTimeUp()
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnLevelTimeUp, //�ð� ������ ó�� �Լ�
		LevelDuration, //�� �� ����
		false //�ݺ�
	);
}

//
void ASpartaGameState::OnLevelTimeUp()
{
	EndLevel();
}

//���� ������ ȣ�� in CoinItem.cpp
void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Your Coin Is %d / %d"), CollectedCoinCount, SpawnedCoinCount);

	//�� �ֿ�� ���� ����
	if (SpawnedCoinCount>0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}

//StartLevel���� ���ǽ����� ȣ��
void ASpartaGameState::EndLevel()
{
	//Ÿ�̸� �ʱ�ȭ
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	//�ν��Ͻ�(������ ���� ī��Ʈ)�� ���� ���� ����
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			AddScore(Score);
			// ���� ���� �ε�����
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
		//todo : ���� 3���� �Ϸ� �� ����
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
						StateText->SetText(FText::FromString(FString::Printf(TEXT("�������� SetFont is �ѱ� Ȯ�ο� Completed"))));
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