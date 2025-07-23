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

	//테이터 테이블임. ItemDataTable의 종류를 에디터에서 결정할 수 있음 / 블루프린트에서는 읽을 수만 있음
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;

	UFUNCTION(BlueprintCallable, Category="Spawning")
	AActor* SpawnRandomItem();

	//테이블의 아이템을 행 형태의 변수로 가져와서 return하는 메서드
	FItemSpawnRow* GetRandomItem() const;
	
	//3차원 랜덤 좌표 반환
	//UFUNCTION(BlueprintCallable, Category = "Spawning")
	FVector GetRandomPointInVolume() const;

	//랜덤 좌표로 아이템 스폰하기
	//UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);

};
