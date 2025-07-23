// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.generated.h"

USTRUCT(BlueprintType) //구조체를 변수로 만들어줄 수 있다.
struct FItemSpawnRow : public FTableRowBase //구조체는 F, FTableRowBase : 사용할 테이블의 행에 이것을 붙일 것이다. 이걸 해야 구조체를 행으로 사용할 수 있다.
{
	GENERATED_BODY()

public:
	//템 이름 (메모리절약 FName)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ItemName;

	//템 클래스 (BP or C++)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	//템 스폰 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spawnchance;
};
