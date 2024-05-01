// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "SPCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class STUDYPROJECT_API ASPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASPCharacter();
	void SetCharacterState(ECharacterState NewState);
	void SetHasKey(bool bState);

	bool GetHasKey() const;
	ECharacterState GetCharacterState() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector DirectionToMove = FVector::ZeroVector;

	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	class USPCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* AttackRangeEffect;

	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void AttackCheck();

	UFUNCTION()
	void AttackDurationEnd();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void OnAssetLoadCompleted();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttackDuration;

	UPROPERTY()
	class USPAnimInstance* SPAnim;

	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State,
		Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State,
		Meta = (AllowPrivateAccess = true))
	bool bIsPlayer;

	UPROPERTY()
	class ASPAIController* SPAIController;

	UPROPERTY()
	class ASPPlayerController* SPPlayerController;

	FTimerHandle AttackTimer;

	bool IsAIControlled;

	int32 AssetIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, 
		Meta = (AllowPrivateAccess = true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle = {};

	bool HasKey = false;
};
