
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
	MineCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));//Collision ������ ����
	MineCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	// Ÿ�̸� �ڵ鷯
	// ���� �� ���� ���� ( Ÿ�̸� �Ŵ����� ���� ) 
	// Ÿ�̸� �ڵ鷯 ��ź�� �ϳ� �ְ� �Ŵ������� ���� in ���
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

	TArray<AActor*> OverlappingActors; //��ģ ���͵� ���� �迭
	MineCollision->GetOverlappingActors(OverlappingActors); //MineCollision�� ��ģ ���͵��� ��� OverlappingActors�� �ִ´�

	//�������� ���� �� player�� ã�Ƽ� Damage ������
	for (AActor* AAc : OverlappingActors)
	{
		if (AAc && AAc->ActorHasTag("Player"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Player Damaged %d by Mine!"), MineDamage));//�α�
			
			//�������� ������ ���� ȣ���ϴ� �Լ� >> �ſ� ���� > ĳ������ TakeDameageȣ��
			UGameplayStatics::ApplyDamage(
				AAc,		//�������� ���� ����
				MineDamage,	//�� ������
				nullptr,	//�������� ������ ��ü > �츮�� nullptr
				this,
				UDamageType::StaticClass() //������ Ÿ��(�� �⺻ ����)
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
