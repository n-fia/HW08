// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

//���÷����� ���� �������̽� - ������ �ʿ� X
//���÷��� �ý����� �� Ŭ������ �ν��ϵ��� �����ִ� Ŭ����
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

//���������Լ� �ۼ��ϱ�
class SPARTAPROJECTTWO_API IItemInterface
{
	GENERATED_BODY()

public:
	//�θ𿡼� UFUNCTION�� �� �ϸ� �������� �� ���÷��� ��ϵ�
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,//�������� �߻��� ���� (�������� Collision ��)
		AActor* OtherActor,					//������ �ε��� �ٸ� �� (ĳ���� ��)
		UPrimitiveComponent* OtherComp,		//������ �ε��� ��ü�� ĸ�� ������Ʈ���� ���� �װ� ����Ű�� ��
		int32 OtherBodyIndex,				//���� �ý���
		bool bFromSweep,					//���� ������
		const FHitResult& SweepResult) = 0;	//���� ������

	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;

	virtual void ActivateItem(AActor* Activator) = 0;
	virtual FName GetItemType() = 0; //Ÿ���� ���� �� �˾Ƴ� ���� FString���� FName�� �� �ӵ� �޸� ����

	//����, ��, ����(big, small)
	//���� : 5�� �� ���� - ������
	//�� : �ٷ� ��
	//���� : �ٷ� ���
};
