#include "SpartaCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "GameFramework/PlayerController.h"

ASpartaCharacter::ASpartaCharacter()
{
	//Actor Init
	PrimaryActorTick.bCanEverTick = true;
	MoveSpeed = 500;
	RotationSpeed = 500;
	bGroundCheck = false;


	//Component Create
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	//Component Tree
	SetRootComponent(CapsuleComp);
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	SpringArmComp->SetupAttachment(CapsuleComp);
	CameraComp->SetupAttachment(SpringArmComp);
	//Physics Off
	CapsuleComp->SetSimulatePhysics(false);
	SkeletalMeshComp->SetSimulatePhysics(false);
	
}

void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();

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


//UE_LOG(LogTemp, Warning, TEXT("Forward : %s"), *Forward.ToString());
//UE_LOG(LogTemp, Warning, TEXT("Right : %s"), *Right.ToString());
//UE_LOG(LogTemp, Warning, TEXT("Up : %s"), *Up.ToString());
//UE_LOG(LogTemp, Warning, TEXT("Normal.X : %f"), Normal.X);
//UE_LOG(LogTemp, Warning, TEXT("Normal.Y : %f"), Normal.Y);
//UE_LOG(LogTemp, Warning, TEXT("Normal.Z : %f"), Normal.Z);
//
//FVector a = Forward * Normal.X;
//FVector b = Forward * Normal.Y;
//
//UE_LOG(LogTemp, Warning, TEXT("Forward * Normal.X : %s"), *a.ToString());
//UE_LOG(LogTemp, Warning, TEXT("Right * Normal.Y : %s"), *b.ToString());
