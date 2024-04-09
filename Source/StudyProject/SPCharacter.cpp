// Fill out your copyright notice in the Description page of Project Settings.


#include "SPCharacter.h"
#include "SPAnimInstance.h"
#include "SPCharacterStatComponent.h"
#include "AttackRangeActor.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASPCharacter::ASPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<USPCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	AttackRangeEffect = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ATTACKRANGEEFFECT"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	AttackRangeEffect->SetupAttachment(GetCapsuleComponent());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_PLANE(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));
	if (SM_PLANE.Succeeded())
	{
		AttackRangeEffect->SetStaticMesh(SM_PLANE.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard"));

	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));

	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
		if (SK_WEAPON.Succeeded())
		{
			Weapon->SetSkeletalMesh(SK_WEAPON.Object);
		}
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MT_ATTACK(TEXT("/Game/StarterContent/Materials/M_AttackEffect.M_AttackEffect"));
	if (MT_ATTACK.Succeeded())
	{
		AttackRangeEffect->SetMaterial(0, MT_ATTACK.Object);
	}
	AttackRangeEffect->SetCollisionProfileName(TEXT("NoCollision"));
	AttackRangeEffect->SetRelativeLocation(FVector(100.0f, 0.0f, -45.0f));
	AttackRangeEffect->SetRelativeScale3D(FVector(3.0f, 1.0f, 1.0f));
	AttackRangeEffect->SetHiddenInGame(true);

	ArmLengthTo = 800.0f;
	ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bDoCollisionTest = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	IsAttacking = false;
	IsAttackDuration = false;
	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("SPCharacter"));
}


// Called when the game starts or when spawned
void ASPCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);
	SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
	if (DirectionToMove.SizeSquared() > 0.0f)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
		AddMovementInput(DirectionToMove);
	}
}

// Called to bind functionality to input
void ASPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASPCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASPCharacter::LeftRight);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ASPCharacter::Attack);
}

void ASPCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SPAnim = Cast<USPAnimInstance>(GetMesh()->GetAnimInstance());

	SPAnim->OnMontageEnded.AddDynamic(this, &ASPCharacter::OnAttackMontageEnded);

	SPAnim->OnAttackHitCheck.AddUObject(this, &ASPCharacter::AttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]()->void {
		UE_LOG(LogTemp, Warning, TEXT("On HP Is Zero"));
		SPAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});
}

float ASPCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void ASPCharacter::UpDown(float NewAxisValue)
{
	DirectionToMove.X = NewAxisValue;
}

void ASPCharacter::LeftRight(float NewAxisValue)
{
	DirectionToMove.Y = NewAxisValue;
}

void ASPCharacter::Attack()
{
	if (IsAttacking) return;
	if (IsAttackDuration) return;
	GetWorldTimerManager().SetTimer(AttackTimer, this, &ASPCharacter::AttackDurationEnd, CharacterStat->GetAttackDuration(), false);
	SPAnim->PlayAttackMontage();
	AttackRangeEffect->SetHiddenInGame(false);
	IsAttacking = true;
}

void ASPCharacter::AttackCheck()
{
	// �浹�� ��ü ����
	FHitResult HitResult;
	// �浹�� ��ü �������� �ڱ� �ڽ��� ����
	FCollisionQueryParams Params(NAME_None, false, this);
	// ä���� ����� �浹�� Ž����
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),
		Params
	);

	// Debug Drawing
#if ENABLE_DRAW_DEBUG
// ĳ���Ͱ� ���� ����
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	// ĸ���� �߽�
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	// ĸ�� ������ ����
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	// ĸ���� ȸ�� ����
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	// �׸� ����� ������ ���ӵǴ� �ð�
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);
#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
			IsAttackDuration = true;
		}
	}
}

void ASPCharacter::AttackDurationEnd()
{
	IsAttackDuration = false;
}

void ASPCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	AttackRangeEffect->SetHiddenInGame(true);
}

