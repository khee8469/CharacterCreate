#include "SpartaCharacter.h"
#include "SpartaGameState.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"


ASpartaCharacter::ASpartaCharacter()
{
	//Actor Init
	PrimaryActorTick.bCanEverTick = true;
	MoveSpeed = 500;
	RotationSpeed = 500;
	bGroundCheck = false;
	MaxHealth = 100.0f;
	Health = MaxHealth;

	//Component Create
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(SkeletalMeshComp);
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);


	//Component Tree
	SetRootComponent(CapsuleComp);
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	SpringArmComp->SetupAttachment(CapsuleComp);
	CameraComp->SetupAttachment(SpringArmComp);
	//Physics Off
	CapsuleComp->SetSimulatePhysics(false);
	SkeletalMeshComp->SetSimulatePhysics(false);
	
}

float ASpartaCharacter::GetHealth() const
{
	return Health;
}

void ASpartaCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}

void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();

	LineTraceLength = CapsuleComp->GetScaledCapsuleHalfHeight()+5;
}

void ASpartaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0,0,LineTraceLength);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		bGroundCheck = true;
	}
	else 
	{
		bGroundCheck = false;
		AddActorWorldOffset(FVector(0, 0, -98 * DeltaTime), true);
	}

	//UKismetSystemLibrary::LineTraceSingle(
	//	this,
	//	Start,
	//	End,
	//	UEngineTypes::ConvertToTraceType(ECC_Visibility),
	//	false,
	//	TArray<AActor*>(),
	//	EDrawDebugTrace::ForDuration, // <- 여기서 디버그 라인 옵션
	//	HitResult,
	//	true,
	//	FLinearColor::Red,  // 히트 시 색상
	//	FLinearColor::Green,// 미스 시 색상
	//	2.0f                // 라인 두께
	//);
}

void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Move
				);
			}
			if (PlayerController->MoveAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::MoveOff
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Look
				);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Jump
				);
			}
		}
	}
}

float ASpartaCharacter::TakeDamage(
	float DamageAmount, 
	FDamageEvent const& DamageEvent, 
	AController* EventInstigator, 
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent,EventInstigator,DamageCauser);

	Health = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);
	UpdateOverheadHP();

	if (Health <= 0)
	{
		OnDeath();
	}

	return ActualDamage;
}

void ASpartaCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller)
	{
		return;
	}

	bMoveing = true;

	FVector Normal = Value.Get<FVector>().GetSafeNormal();
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float MoveXSpeed = Normal.X * MoveSpeed * DeltaTime;
	float MoveYSpeed = Normal.Y * MoveSpeed * DeltaTime;
	float MoveZSpeed = Normal.Z * MoveSpeed * DeltaTime;

	if (!FMath::IsNearlyZero(Normal.X) || !FMath::IsNearlyZero(Normal.Y)
		|| !FMath::IsNearlyZero(Normal.Z))
	{
		FVector StartPos = GetActorLocation();

		if (bGroundCheck) 
		{
			//로컬 확인
			FRotator ControlRot = Controller->GetControlRotation();

			FRotator YawRot = FRotator(0, ControlRot.Yaw, 0);
			FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
			FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
			FVector Up = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Z);


			// 월드 확인
			//FVector MoveDir = (Forward * Normal.X) + (Right * Normal.Y) + Normal.Z;
			FVector MoveDirForward = Forward * Normal.X;
			FVector MoveDirRight = Right * Normal.Y;
			FVector MoveDirUp = Up * Normal.Z;
			FVector MoveDir = MoveDirForward+  MoveDirRight+ MoveDirUp;

			AddActorLocalOffset(MoveDir * MoveSpeed * DeltaTime, true);

			FRotator TargetRot = (GetActorLocation() - StartPos).GetSafeNormal().Rotation();
			SkeletalMeshComp->SetRelativeRotation(TargetRot - FRotator(0, 90, 0));
		}
		else
		{
			FRotator ControlRot = Controller->GetControlRotation();
			//FRotator YawRot = FRotator(0, ControlRot.Yaw, 0);


			FVector Forward = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::X);
			FVector Right = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y);
			FVector Up = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Z);

			//FVector MoveDir = Forward * Normal.X + Right * Normal.Y + Normal.Z;

			FVector MoveDirForward = Forward * Normal.X;
			FVector MoveDirRight = Right * Normal.Y;
			FVector MoveDirUp = Up * Normal.Z;
			FVector MoveDir = MoveDirForward + MoveDirRight + MoveDirUp;

			AddActorLocalOffset(MoveDir * MoveSpeed * DeltaTime, true);

			FRotator TargetRot = (GetActorLocation() - StartPos).GetSafeNormal().Rotation();
			SkeletalMeshComp->SetRelativeRotation(FRotator(0, TargetRot.Yaw - 90, 0));
		}
	}
}

void ASpartaCharacter::MoveOff(const FInputActionValue& value)
{
	bMoveing = false;
}

void ASpartaCharacter::Look(const FInputActionValue& Value)
{
	if (!Controller)
	{
		return;
	}
	FVector Normal = Value.Get<FVector>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float RotXSpeed = Normal.X * RotationSpeed * DeltaTime;
	float RotYSpeed = Normal.Y * RotationSpeed * DeltaTime;
	float RotZSpeed = Normal.Z * RotationSpeed * DeltaTime;
	
	if (!FMath::IsNearlyZero(Normal.X) || !FMath::IsNearlyZero(Normal.Y) 
		|| !FMath::IsNearlyZero(Normal.Z))
	{
		FRotator ConRotation = Controller->GetControlRotation();

		ConRotation.Yaw += RotXSpeed;
		ConRotation.Pitch += RotYSpeed;
		ConRotation.Roll += RotZSpeed;

		if (bGroundCheck)
		{
			Controller->SetControlRotation(FRotator(ConRotation.Pitch, ConRotation.Yaw, 0));
			SpringArmComp->SetWorldRotation(FRotator(ConRotation.Pitch, ConRotation.Yaw, 0));
		    //SetActorRelativeRotation(Controller->GetControlRotation());
		}
		else
		{
			Controller->SetControlRotation(ConRotation);
			SpringArmComp->SetWorldRotation(ConRotation);
			//SetActorRelativeRotation(FRotator(0,0, Controller->GetControlRotation().Roll));
		}
	}
}

void ASpartaCharacter::Jump(const FInputActionValue& Value)
{

}

void ASpartaCharacter::OnDeath()
{
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

void ASpartaCharacter::UpdateOverheadHP()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}
}
