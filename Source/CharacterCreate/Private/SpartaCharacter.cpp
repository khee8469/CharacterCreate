#include "SpartaCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "GameFramework/PlayerController.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

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
	
	//SpringArmComp->bUsePawnControlRotation = true;
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
		UE_LOG(LogTemp, Warning, TEXT("Ground"));
	}
	else
	{
		bGroundCheck = false;
		UE_LOG(LogTemp, Warning, TEXT("Fly"));
	}

	UKismetSystemLibrary::LineTraceSingle(
		this,
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForDuration, // <- 여기서 디버그 라인 옵션
		HitResult,
		true,
		FLinearColor::Red,  // 히트 시 색상
		FLinearColor::Green,// 미스 시 색상
		2.0f                // 지속시간?
	);
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
	FVector Normal = Value.Get<FVector>().GetSafeNormal();
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float MoveXSpeed = Normal.X * MoveSpeed * DeltaTime;
	float MoveYSpeed = Normal.Y * MoveSpeed * DeltaTime;
	float MoveZSpeed = Normal.Z * MoveSpeed * DeltaTime;

	if (!FMath::IsNearlyZero(Normal.X) || !FMath::IsNearlyZero(Normal.Y) 
		|| !FMath::IsNearlyZero(Normal.Z))
	{
		AddActorLocalOffset(FVector(MoveXSpeed, MoveYSpeed, MoveZSpeed));
	}
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

		Controller->SetControlRotation(ConRotation);
		SpringArmComp->SetWorldRotation(ConRotation);
		SetActorRotation(FRotator(0, ConRotation.Yaw, 0));
	}
}

void ASpartaCharacter::Jump(const FInputActionValue& Value)
{
	bool JumpInput = Value.Get<bool>();
}


