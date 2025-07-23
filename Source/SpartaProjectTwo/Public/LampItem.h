#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "LampItem.generated.h"

UCLASS()
class SPARTAPROJECTTWO_API ALampItem : public ABaseItem
{
	GENERATED_BODY()
public:
	ALampItem();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> InterectWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* InterectWidgetInstance;

	//test
	//UFUNCTION(BlueprintCallable, Category = "HUD")
	//UUserWidget* GetHUDWidget() const;
	//이거 왜 보여줌
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowLampInterectHUD();

private:
	virtual void ActivateItem(AActor* Activator) override;
	UUserWidget* HUDWidget;
};
