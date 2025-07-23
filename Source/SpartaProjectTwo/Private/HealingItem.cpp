// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"
#include "SpartaCharacter.h"

AHealingItem::AHealingItem()
{
	ItemType = "Heal";
	HealValue = 15.0f;
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Cyan,
			FString::Printf(TEXT("Player Gained %.1f HP!"), HealValue));

		Super::ActivateItem(Activator);

		// Activator가 ASpartaCharacter 타입(또는 그 자식 클래스)인지 확인해서 맞으면 형변환, 아니면 nullptr을 반환
		if (ASpartaCharacter* PlayerCharacter = Cast< ASpartaCharacter>(Activator)) {
			PlayerCharacter->AddHealth(HealValue);
		}
		
		//월드에서 템 삭제
		DestroyItem();
	}
}
