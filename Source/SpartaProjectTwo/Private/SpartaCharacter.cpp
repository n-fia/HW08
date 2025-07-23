#include "SpartaCharacter.h"
#include "EnhancedInputComponent.h"
#include "SpartaPlayerController.h"
#include "Camera/CameraComponent.h"
#include "SpartaGameState.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"


//������
ASpartaCharacter::ASpartaCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	(*SpringArmComp).SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 350.0f;
	SpringArmComp->bUsePawnControlRotation = true; //��Ʈ�ѷ��� ȸ���� ���� �������ϵ� ���� ȸ���϶�

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraC"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName); // SpringArmComp�� ��Ĺ�� �� �κп� ������Ʈ�� ����?�Ѵ�
	CameraComp->bUsePawnControlRotation = false; //�ﰢ�븸 �����̰� ī�޶�� �������ּ���

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier=1.7f;
	SprintSpeed= NormalSpeed * SprintSpeedMultiplier;

	IsDebuff = false;

	(*GetCharacterMovement()).MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	MaxHealth = 100.0f;
	Health = MaxHealth;

	FullHeartTexture = nullptr;
	EmptyHeartTexture = nullptr;
}

void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//�Է��� �����ϴ� PlayerInputComponent ������Ʈ(�پ�)�� EnhancedInput ������Ʈ Ÿ������ �����Ͽ� �����ϰ� ��밡��
	if(UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//��Ʈ�ѷ� ������
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController())) 
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction( //�����ϴ� �ٽ� �ڵ�
					PlayerController->MoveAction, //�̵��� IA_Move ������
					ETriggerEvent::Triggered, //Ű�� ������ �̺�Ʈ�� �߻�
					this, //�̺�Ʈ�� �߻��ϸ� �� ��ü��
					&ASpartaCharacter::Move //Move�Լ��� ȣ���Ͽ� �����Ѵ�.
				);
			}
			//����
			if (PlayerController->JumpAction) 
			{
				EnhancedInput->BindAction( //�����ϴ� �ٽ� �ڵ�
					PlayerController->JumpAction, //������ IA_Jump ������
					ETriggerEvent::Triggered, //Ű�� ������ �̺�Ʈ�� �߻�
					this, //�̺�Ʈ�� �߻��ϸ� �� ��ü��
					&ASpartaCharacter::StartJump//Jump�Լ��� ȣ���Ͽ� �����Ѵ�.
				);
				EnhancedInput->BindAction( //�����ϴ� �ٽ� �ڵ�
					PlayerController->JumpAction, //������ IA_Jump ������
					ETriggerEvent::Completed, //Ű ������ ������ �̺�Ʈ�� �߻�
					this, //�̺�Ʈ�� �߻��ϸ� �� ��ü��
					&ASpartaCharacter::EndJump //Jump�Լ��� ȣ���Ͽ� �����Ѵ�.
				);
			}
			//���� ȸ��
			if (PlayerController->LookAction) 
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Look
				);
			}
			//�޸���
			if (PlayerController->SprintAction) 
			{
				EnhancedInput->BindAction( //�����ϴ� �ٽ� �ڵ�
					PlayerController->SprintAction, //�޸��� IA
					ETriggerEvent::Triggered, //Ű�� ������ �̺�Ʈ�� �߻�
					this, //�̺�Ʈ�� �߻��ϸ� �� ��ü��
					&ASpartaCharacter::StartSprint //Move�Լ��� ȣ���Ͽ� �����Ѵ�.
				);
				EnhancedInput->BindAction(
					PlayerController->SprintAction, 
					ETriggerEvent::Completed, 
					this,
					&ASpartaCharacter::EndSprint 
				);
			}
		}
	}

}

float ASpartaCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigetor, AActor* DamageCauser)
{
	//TakeDamage() >> �����̳� ���� �ؼ� ��������� ������ ( �ʼ� �ƴ� )
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigetor, DamageCauser);

	//Clamp() >> 0.0f : �ּҵ����� , MaxHealth : �ְ�����
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateHPWidget();

	UE_LOG(LogTemp, Warning, TEXT("Health decreased to : %f"), Health);

	if (Health <= 0.0f) {
		OnDeath();

		UE_LOG(LogTemp, Warning, TEXT("You are death"));
	}

	return ActualDamage;
}

void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHPWidget();
}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	const FVector2D MoveInput = value.Get<FVector2D>();

	//GetControllerForwardVector()�� ���ؼ� maove���� ���� + Rotation() �߰�
	FRotator ControllerRotation = GetControlRotation();
	FVector GoVectorX = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
	FVector GoVectorY = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::Y);
	//FRotator GoRotation = GoVectorX.Rotation();//test

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GoVectorX, MoveInput.X); //GetActorForwardVector() : ���� ���� ���� ������ / AddMovementInput(a , b) : a�������� b��ŭ �̵���Ŵ
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GoVectorY, MoveInput.Y); // GetActorRightVector() : ������ ���� ���� ������
	}

	//UE_LOG(LogTemp,Warning,TEXT("ActorRotation: %f, %f"), GoVectorX.X, GoVectorY.Y)

	//GetMesh()->AddWorldRotation(FRotator(GoVectorY.Y*MoveInput.Y * NormalSpeed * GetWorld()->GetDeltaSeconds(), GoVectorX.X* MoveInput.X * NormalSpeed*GetWorld()->GetDeltaSeconds(), 0.0f ));
}

void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ASpartaCharacter::EndJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		StopJumping();
	}
}

void ASpartaCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	
	AddControllerYawInput(LookInput.X); //��� : ������ / ���� : ����  >>�¿� ȸ��
	AddControllerPitchInput(LookInput.Y); //����ȸ��(IA���� ����)
}

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	if (!IsDebuff)
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
	}
}

void ASpartaCharacter::EndSprint(const FInputActionValue& value)
{
	if (IsDebuff) return;
	MoveSpeedOrigin();
}

void ASpartaCharacter::MoveSpeedDown(const float SpeedDownRate)
{
	if (GetCharacterMovement())
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveSpeedDown"));
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed*SpeedDownRate;
		IsDebuff = true;
	}
}

void ASpartaCharacter::MoveSpeedOrigin()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		IsDebuff = false;
	}
}

void ASpartaCharacter::OnDeath()
{
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

float ASpartaCharacter::GetHealth() const {
	return Health;
}

void ASpartaCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateHPWidget();
}

void ASpartaCharacter::UpdateHPWidget()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController);
		if (SpartaPlayerController)
		{
			UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget();
			if (HUDWidget)
			{
				for (int i = 1; i <= 10; ++i)
				{
					FString WidgetString = FString::Printf(TEXT("HeartImage%d"), i);
					FName WidgetName(*WidgetString);
					if (UImage* HeartImage = Cast<UImage>(HUDWidget->GetWidgetFromName(WidgetName)))
					{
						if(i<= Health / 10.0f)
						{
							// �� �� ��Ʈ
							HeartImage->SetBrushFromTexture(FullHeartTexture);
						}
						else
						{
							// �� ��Ʈ
							HeartImage->SetBrushFromTexture(EmptyHeartTexture);
						}
					}
				}
			}
		}
	}
}

