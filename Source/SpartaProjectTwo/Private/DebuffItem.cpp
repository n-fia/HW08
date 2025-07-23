#include "DebuffItem.h"
#include "SpartaCharacter.h"

ADebuffItem::ADebuffItem() : DownRate(0.5f), DownSecond(5.0f) {}

void ADebuffItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	// MoveSpeedDown() 함수 호출 > EndSprint() 호출
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
		//월드에서 템 삭제
		DestroyItem();
	}
}