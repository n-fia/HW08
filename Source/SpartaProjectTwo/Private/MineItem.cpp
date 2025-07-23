
#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	MineParticle = nullptr;
	MineSound = nullptr;

	ItemType = "Mine";
	MineTimeDelay = 3.0f;
	MineRadius = 300.f;
	MineDamage = 30;
	bHasExploded = false;

	MineCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MineCollision"));
	MineCollision->InitSphereRadius(MineRadius);
	MineCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));//Collision 프리셋 지정
	MineCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	// 타이머 핸들러
	// 여러 개 존재 가능 ( 타이머 매니저가 관리 ) 
	// 타이머 핸들러 폭탄에 하나 넣고 매니저에서 관리 in 헤더
	if (!bHasExploded)
	{
		GetWorld()->GetTimerManager().SetTimer(
			MineTimerHandle, this, &AMineItem::Explode, MineTimeDelay, false);
	}
	bHasExploded = true;
}

void AMineItem::Explode()
{
	UParticleSystemComponent* Particle = nullptr;

	if (MineParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			MineParticle,
			GetActorLocation(),
			GetActorRotation(),
			false
		);
	}

	if (MineSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			MineSound,
			GetActorLocation()
		);
	}

	TArray<AActor*> OverlappingActors; //겹친 액터들 저장 배열
	MineCollision->GetOverlappingActors(OverlappingActors); //MineCollision에 겹친 액터들을 모두 OverlappingActors에 넣는다

	//오버랩된 액터 중 player만 찾아서 Damage 입히기
	for (AActor* AAc : OverlappingActors)
	{
		if (AAc && AAc->ActorHasTag("Player"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Player Damaged %d by Mine!"), MineDamage));//로그
			
			//데미지를 입히는 것이 호출하는 함수 >> 매우 유용 > 캐릭터의 TakeDameage호출
			UGameplayStatics::ApplyDamage(
				AAc,		//데미지를 받을 액터
				MineDamage,	//줄 데미지
				nullptr,	//데미지를 유발한 주체 > 우리는 nullptr
				this,
				UDamageType::StaticClass() //데미지 타입(의 기본 유형)
			);
		}
	}

	DestroyItem();
	
	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;
		TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;
		if (WeakParticle.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(
				DestroyParticleTimerHandle,
				[WeakParticle]()
				{
					if (WeakParticle.IsValid())
					{
						WeakParticle->DestroyComponent();
					}
				},
				2.0f, false
			);
		}

	}
}
