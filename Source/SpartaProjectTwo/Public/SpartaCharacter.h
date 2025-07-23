#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;

UCLASS()
class SPARTAPROJECTTWO_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASpartaCharacter();

	// 객체 정보를 수정 X // 내부 속성 정도는 수정할 수 있다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount); //힐포션과 지뢰로 체력 변경

	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void MoveSpeedDown(const float SpeedDownRate);

	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void MoveSpeedOrigin();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hearts")
	UTexture2D* FullHeartTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hearts")
	UTexture2D* EmptyHeartTexture;


	//IA에다가 함수를 연결해 줄 장소
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//체력 변경
	virtual float TakeDamage(float DamageAmount,	//기본 데미지
		struct FDamageEvent const& DamageEvent,		//데미지 유형 정보(확장할 때 원소의 속성 등)
		AController* EventInstigetor,				//지뢰를 심은 사람? - 데미지를 발생시키는 주체 > 이번에는 null
		AActor* DamageCauser) override;				//데미지를 일으킨 오브젝트(지뢰)

	void BeginPlay() override;

	UFUNCTION()
	void Move(const FInputActionValue& value); //왜 참조? 구조체는 크기가 커서.
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void EndJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void EndSprint(const FInputActionValue& value); 

	void OnDeath();
	void UpdateHPWidget();

private:
	//왜 노출 X
	float NormalSpeed; //기준 속도
	float SprintSpeedMultiplier; //기준 속도의 f배
	float SprintSpeed;//NormalSpeed * SprintSpeedMultiplier
	bool IsDebuff;
};
