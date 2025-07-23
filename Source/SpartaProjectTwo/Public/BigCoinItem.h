// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoinItem.h"
#include "BigCoinItem.generated.h"

UCLASS()
class SPARTAPROJECTTWO_API ABigCoinItem : public ACoinItem
{
	GENERATED_BODY()
	
public:
	ABigCoinItem();

	//virtual void OnItemOverlap(AActor* OverlapActor) override;
	//virtual void OnItemEndOverlap(AActor* OverlapActor)override;
	virtual void ActivateItem(AActor* Activator) override;
	//virtual FName GetItemType() override; //타입형 같은 걸 알아낼 때는 FString보다 FName이 더 속도 메모리 좋음
};
