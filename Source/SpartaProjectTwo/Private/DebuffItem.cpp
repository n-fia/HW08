#include "DebuffItem.h"
#include "SpartaCharacter.h"

ADebuffItem::ADebuffItem() : DownRate(0.5f), DownSecond(5.0f) {}

void ADebuffItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	// MoveSpeedDown() �Լ� ȣ�� > EndSprint() ȣ��
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast< ASpartaCharacter>(Activator)) {
			PlayerCharacter->MoveSpeedDown(DownRate);

			FTimerHandle DebuffTimerHandle;
			TWeakObjectPtr<ASpartaCharacter> WeakPlayerPtr = PlayerCharacter;

			GetWorld()->GetTimerManager().SetTimer(
				DebuffTimerHandle,
				[WeakPlayerPtr]()
				{
					if (WeakPlayerPtr.IsValid())
					{
						WeakPlayerPtr->MoveSpeedOrigin();
					}
				},
				DownSecond, false
			);
		}
		//���忡�� �� ����
		DestroyItem();
	}
}