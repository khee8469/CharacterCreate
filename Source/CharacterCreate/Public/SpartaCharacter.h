#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpartaCharacter.generated.h"

class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;


UCLASS()
class CHARACTERCREATE_API ASpartaCharacter : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;


public:
	ASpartaCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller Value")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller Value")
	float RotationSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller Value")
	bool bMoveing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller Value")
	bool bGroundCheck;

	FHitResult HitResult;
	float LineTraceLength;
	


	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void MoveOff(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void Jump(const FInputActionValue& value);


};
