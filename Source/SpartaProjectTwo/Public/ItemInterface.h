// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

//리플렉션을 위한 인터페이스 - 수정할 필요 X
//리플렉션 시스템이 이 클래스를 인식하도록 도와주는 클래스
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

//순수가상함수 작성하기
class SPARTAPROJECTTWO_API IItemInterface
{
	GENERATED_BODY()

public:
	//부모에서 UFUNCTION한 번 하면 나머지도 다 리플렉션 등록됨
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,//오버랩이 발생한 본인 (아이템의 Collision 등)
		AActor* OtherActor,					//나한테 부딪힌 다른 거 (캐릭터 등)
		UPrimitiveComponent* OtherComp,		//나한테 부딪힌 객체의 캡슐 컴포넌트같이 물리 그걸 일으키는 거
		int32 OtherBodyIndex,				//물리 시스템
		bool bFromSweep,					//ㅁㄹ ㅅㅅㅌ
		const FHitResult& SweepResult) = 0;	//ㅁㄹ ㅅㅅㅌ

	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;

	virtual void ActivateItem(AActor* Activator) = 0;
	virtual FName GetItemType() = 0; //타입형 같은 걸 알아낼 때는 FString보다 FName이 더 속도 메모리 좋음

	//지뢰, 힐, 코인(big, small)
	//지뢰 : 5초 뒤 폭발 - 오버랩
	//힐 : 바로 힐
	//코인 : 바로 사용
};
