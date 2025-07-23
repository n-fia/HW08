// 무작위 폭발 지점 아이템


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
	TArray<AActor*> OverlappingActors; //겹친 액터들 저장 배열
	if (USphereComponent* BoomCollision = Cast< USphereComponent>(Collision))
	{
		BoomCollision->GetOverlappingActors(OverlappingActors); //MineCollision에 겹친 액터들을 모두 OverlappingActors에 넣는다
	}

	//오버랩된 액터 중 player만 찾아서 Damage 입히기
	for (AActor* AAc : OverlappingActors)
	{
		if (AAc && AAc->ActorHasTag("Player"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Player Damaged %d by Mine!"), BoomDamage));//로그

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