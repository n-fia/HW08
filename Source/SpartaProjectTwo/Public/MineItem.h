// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

UCLASS()
class SPARTAPROJECTTWO_API AMineItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AMineItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* MineCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* MineParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* MineSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float MineTimeDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float MineRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MineDamage;
	
	bool bHasExploded;
	FTimerHandle MineTimerHandle;

	virtual void ActivateItem(AActor* Activator) override;
	void Explode(); //폭발 상황 지정

};
