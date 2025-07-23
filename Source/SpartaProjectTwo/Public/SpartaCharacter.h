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

	// ��ü ������ ���� X // ���� �Ӽ� ������ ������ �� �ִ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount); //�����ǰ� ���ڷ� ü�� ����

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


	//IA���ٰ� �Լ��� ������ �� ���
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//ü�� ����
	virtual float TakeDamage(float DamageAmount,	//�⺻ ������
		struct FDamageEvent const& DamageEvent,		//������ ���� ����(Ȯ���� �� ������ �Ӽ� ��)
		AController* EventInstigetor,				//���ڸ� ���� ���? - �������� �߻���Ű�� ��ü > �̹����� null
		AActor* DamageCauser) override;				//�������� ����Ų ������Ʈ(����)

	void BeginPlay() override;

	UFUNCTION()
	void Move(const FInputActionValue& value); //�� ����? ����ü�� ũ�Ⱑ Ŀ��.
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
	//�� ���� X
	float NormalSpeed; //���� �ӵ�
	float SprintSpeedMultiplier; //���� �ӵ��� f��
	float SprintSpeed;//NormalSpeed * SprintSpeedMultiplier
	bool IsDebuff;
};
