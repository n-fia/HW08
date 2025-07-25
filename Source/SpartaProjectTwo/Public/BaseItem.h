// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"

#include "BaseItem.generated.h"

class USphereComponent;

UCLASS()
class SPARTAPROJECTTWO_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Item|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* PickupParticle;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* PickupSound;

	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,					
		UPrimitiveComponent* OtherComp,		
		int32 OtherBodyIndex,				
		bool bFromSweep,					
		const FHitResult& SweepResult) override;

	virtual void OnItemEndOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;

	virtual void ActivateItem(AActor* Activator) override;
	//타입형 같은 걸 알아낼 때는 FString보다 FName이 더 속도 메모리 좋음
	virtual FName GetItemType() override; 
	virtual void DestroyItem();
};