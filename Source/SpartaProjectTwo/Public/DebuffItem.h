#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "DebuffItem.generated.h"

UCLASS()
class SPARTAPROJECTTWO_API ADebuffItem : public ABaseItem
{
	GENERATED_BODY()
public:
	ADebuffItem();

	//todo : �̵��ӵ��� DownRate%�� �Ǵ� ������
	virtual void ActivateItem(AActor* Activator) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float DownRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float DownSecond;
};
