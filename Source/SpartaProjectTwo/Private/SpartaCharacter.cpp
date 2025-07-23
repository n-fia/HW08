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


//생성자
ASpartaCharacter::ASpartaCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	(*SpringArmComp).SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 350.0f;
	SpringArmComp->bUsePawnControlRotation = true; //컨트롤러가 회전할 ㄸㅐ 스프링암도 같이 회전하라

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraC"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName); // SpringArmComp의 소캣의 끝 부분에 컴포넌트를 장착?한다
	CameraComp->bUsePawnControlRotation = false; //삼각대만 움직이고 카메라는 고정해주세요

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
	//입력을 관리하는 PlayerInputComponent 컴포넌트(다양)를 EnhancedInput 컴포넌트 타입으로 변경하여 안전하게 사용가능
	if(UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//컨트롤러 가져옴
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController())) 
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction( //연결하는 핵심 코드
					PlayerController->MoveAction, //이동용 IA_Move 가져옴
					ETriggerEvent::Triggered, //키가 눌려서 이벤트가 발생
					this, //이벤트가 발생하면 이 객체의
					&ASpartaCharacter::Move //Move함수를 호출하여 실행한다.
				);
			}
			//점프
			if (PlayerController->JumpAction) 
			{
				EnhancedInput->BindAction( //연결하는 핵심 코드
					PlayerController->JumpAction, //점프용 IA_Jump 가져옴
					ETriggerEvent::Triggered, //키가 눌려서 이벤트가 발생
					this, //이벤트가 발생하면 이 객체의
					&ASpartaCharacter::StartJump//Jump함수를 호출하여 실행한다.
				);
				EnhancedInput->BindAction( //연결하는 핵심 코드
					PlayerController->JumpAction, //점프용 IA_Jump 가져옴
					ETriggerEvent::Completed, //키 눌림이 끝나면 이벤트가 발생
					this, //이벤트가 발생하면 이 객체의
					&ASpartaCharacter::EndJump //Jump함수를 호출하여 실행한다.
				);
			}
			//시점 회전
			if (PlayerController->LookAction) 
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Look
				);
			}
			//달리기
			if (PlayerController->SprintAction) 
			{
				EnhancedInput->BindAction( //연결하는 핵심 코드
					PlayerController->SprintAction, //달리기 IA
					ETriggerEvent::Triggered, //키가 눌려서 이벤트가 발생
					this, //이벤트가 발생하면 이 객체의
					&ASpartaCharacter::StartSprint //Move함수를 호출하여 실행한다.
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
	//TakeDamage() >> 방어력이나 뭔가 해서 최종변경된 데미지 ( 필수 아님 )
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigetor, DamageCauser);

	//Clamp() >> 0.0f : 최소데미지 , MaxHealth : 최고데미지
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

	//GetControllerForwardVector()와 비교해서 maove방향 수정 + Rotation() 추가
	FRotator ControllerRotation = GetControlRotation();
	FVector GoVectorX = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
	FVector GoVectorY = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::Y);
	//FRotator GoRotation = GoVectorX.Rotation();//test

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GoVectorX, MoveInput.X); //GetActorForwardVector() : 정면 방향 벡터 가져옴 / AddMovementInput(a , b) : a방향으로 b만큼 이동시킴
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GoVectorY, MoveInput.Y); // GetActorRightVector() : 오른쪽 방향 벡터 가져옴
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
	
	AddControllerYawInput(LookInput.X); //양수 : 오른쪽 / 음수 : 왼쪽  >>좌우 회전
	AddControllerPitchInput(LookInput.Y); //상하회전(IA에서 반전)
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
							// 꽉 찬 하트
							HeartImage->SetBrushFromTexture(FullHeartTexture);
						}
						else
						{
							// 빈 하트
							HeartImage->SetBrushFromTexture(EmptyHeartTexture);
						}
					}
				}
			}
		}
	}
}

