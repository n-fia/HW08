// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"

#include "SpawnVolume.generated.h"

class USceneComponent;
class UBoxComponent;

UCLASS()
class SPARTAPROJECTTWO_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category="Spawning")
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	//������ ���̺���. ItemDataTable�� ������ �����Ϳ��� ������ �� ���� / �������Ʈ������ ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;

	UFUNCTION(BlueprintCallable, Category="Spawning")
	AActor* SpawnRandomItem();

	//���̺��� �������� �� ������ ������ �����ͼ� return�ϴ� �޼���
	FItemSpawnRow* GetRandomItem() const;
	
	//3���� ���� ��ǥ ��ȯ
	//UFUNCTION(BlueprintCallable, Category = "Spawning")
	FVector GetRandomPointInVolume() const;

	//���� ��ǥ�� ������ �����ϱ�
	//UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);

};
