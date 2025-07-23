#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "SpartaGameInstance.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneC"));
	SetRootComponent(SceneComp);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningB"));
	SpawningBox->SetupAttachment(SceneComp);

	ItemDataTable = nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	//중심부터 끝까지의 거리를 계산
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();//중심좌표

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get()) //TSoftPtr<>타입으로 클래스 타입을 가져옴
		{
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;

	TArray<FItemSpawnRow*> AllRows;

	static const FString ContextString(TEXT("ItemSpqwnContext"));

	ItemDataTable->GetAllRows(ContextString, AllRows); //못 가져오면 ContextString실행

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	//행이 하나하나 비었는지 확인 후 각 Row의 확률을 더함
	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->Spawnchance;
		}
	}

	int32 MaxNum = 500;
	while (MaxNum--)
	{
		const float RandValue = FMath::FRandRange(0.0f, TotalChance);
		float ChanceValue = 0.0f;

		for (FItemSpawnRow* Row : AllRows)
		{
			ChanceValue += Row->Spawnchance;
			if (RandValue <= ChanceValue)
			{
				UE_LOG(LogTemp, Warning, TEXT("Row Name is %s"), *(Row->ItemName).ToString())
				if (Row->ItemName == FName("RandomBoom"))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
						{
							if (SpartaGameInstance->CurrentLevelIndex == 2)
							{
								return Row;
							}
							else continue;
						}
					}
				}
				UE_LOG(LogTemp, Warning, TEXT("Row is %s"), *(Row->ItemName).ToString())
				return Row;
			}
		}
	}
	return nullptr; //안전
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(), // 위치
		FRotator::ZeroRotator //회전은 0(기본)
	);
	return SpawnedActor;
}
