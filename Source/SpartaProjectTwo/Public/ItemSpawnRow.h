// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.generated.h"

USTRUCT(BlueprintType) //����ü�� ������ ������� �� �ִ�.
struct FItemSpawnRow : public FTableRowBase //����ü�� F, FTableRowBase : ����� ���̺��� �࿡ �̰��� ���� ���̴�. �̰� �ؾ� ����ü�� ������ ����� �� �ִ�.
{
	GENERATED_BODY()

public:
	//�� �̸� (�޸����� FName)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ItemName;

	//�� Ŭ���� (BP or C++)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	//�� ���� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spawnchance;
};
