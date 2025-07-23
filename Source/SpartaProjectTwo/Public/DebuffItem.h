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

	//todo : 이동속도가 DownRate%가 되는 아이템
	virtual void ActivateItem(AActor* Activator) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float DownRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float DownSecond;
};
