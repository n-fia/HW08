// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "RandomBoom.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECTTWO_API ARandomBoom : public ABaseItem
{
public:

	GENERATED_BODY()
	ARandomBoom();

	void Explode();

	void ActivateItem(AActor* Activator);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Boom")
	float BoomDamage;
};
