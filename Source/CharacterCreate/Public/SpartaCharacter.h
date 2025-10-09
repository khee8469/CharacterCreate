#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpartaCharacter.generated.h"

class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
class UWidgetComponent;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void DebuffSpeed();
	UFUNCTION(BlueprintCallable, Category = "Item")
	void ReverseControl();
	


protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

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

	
	float SaveMoveSpeed;



	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void MoveOff(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void Jump(const FInputActionValue& value);

	void OnDeath();
	void UpdateOverheadHP();

	void HalfSpeed();
	void DoubleSpeed();
	void ReverseSpeed();
};
