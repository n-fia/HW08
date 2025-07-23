#include "CoinItem.h"
#include "Engine/World.h"
#include "SpartaGameState.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DfaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		//GEngine->AddOnScreenDebugMessage(
		//	-1,
		//	3.0f,
		//	FColor::Cyan,
		//	FString::Printf(TEXT("Player Gained %d points!"), PointValue));

		if (UWorld* World = GetWorld()) 
		{
			if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					3.0f,
					FColor::Cyan,
					FString::Printf(TEXT("Player Gained %d points!"), PointValue));
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}
		DestroyItem();
	}
}
