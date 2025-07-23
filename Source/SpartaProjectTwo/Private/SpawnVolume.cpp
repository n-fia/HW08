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
	//�߽ɺ��� �������� �Ÿ��� ���
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();//�߽���ǥ

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
		if (UClass* ActualClass = SelectedRow->ItemClass.Get()) //TSoftPtr<>Ÿ������ Ŭ���� Ÿ���� ������
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

	ItemDataTable->GetAllRows(ContextString, AllRows); //�� �������� ContextString����

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	//���� �ϳ��ϳ� ������� Ȯ�� �� �� Row�� Ȯ���� ����
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
	return nullptr; //����
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(), // ��ġ
		FRotator::ZeroRotator //ȸ���� 0(�⺻)
	);
	return SpawnedActor;
}
