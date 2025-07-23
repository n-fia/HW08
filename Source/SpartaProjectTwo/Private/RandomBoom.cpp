// ������ ���� ���� ������


#include "RandomBoom.h"
#include "Components/SphereComponent.h"
#include "SpartaCharacter.h"//
#include "Kismet/GameplayStatics.h"

ARandomBoom::ARandomBoom() 
{
	BoomDamage = 20.0f;
}

void ARandomBoom::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	Explode();
}

void ARandomBoom::Explode()
{
	TArray<AActor*> OverlappingActors; //��ģ ���͵� ���� �迭
	if (USphereComponent* BoomCollision = Cast< USphereComponent>(Collision))
	{
		BoomCollision->GetOverlappingActors(OverlappingActors); //MineCollision�� ��ģ ���͵��� ��� OverlappingActors�� �ִ´�
	}

	//�������� ���� �� player�� ã�Ƽ� Damage ������
	for (AActor* AAc : OverlappingActors)
	{
		if (AAc && AAc->ActorHasTag("Player"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Player Damaged %d by Mine!"), BoomDamage));//�α�

			UGameplayStatics::ApplyDamage(
				AAc,
				BoomDamage,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}
	DestroyItem();
}