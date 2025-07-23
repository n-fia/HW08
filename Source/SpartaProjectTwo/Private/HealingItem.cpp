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

		// Activator�� ASpartaCharacter Ÿ��(�Ǵ� �� �ڽ� Ŭ����)���� Ȯ���ؼ� ������ ����ȯ, �ƴϸ� nullptr�� ��ȯ
		if (ASpartaCharacter* PlayerCharacter = Cast< ASpartaCharacter>(Activator)) {
			PlayerCharacter->AddHealth(HealValue);
		}
		
		//���忡�� �� ����
		DestroyItem();
	}
}
