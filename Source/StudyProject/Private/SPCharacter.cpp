// Fill out your copyright notice in the Description page of Project Settings.


#include "SPCharacter.h"
#include "SPAnimInstance.h"
#include "SPAIController.h"
#include "SPCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "SPCharacterWidget.h"
#include "DrawDebugHelpers.h"
#include "SPPlayerController.h"
#include "SPCharacterSetting.h"
#include "SPGameInstance.h"

// Sets default values
ASPCharacter::ASPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ASPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<USPCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	AttackRangeEffect = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ATTACKRANGEEFFECT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());
	AttackRangeEffect->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_PLANE(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));
	if (SM_PLANE.Succeeded())
	{
		AttackRangeEffect->SetStaticMesh(SM_PLANE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MT_ATTACK(TEXT("/Game/StarterContent/Materials/M_AttackEffect.M_AttackEffect"));
	if (MT_ATTACK.Succeeded())
	{
		AttackRangeEffect->SetMaterial(0, MT_ATTACK.Object);
	}
	AttackRangeEffect->SetCollisionProfileName(TEXT("NoCollision"));
	AttackRangeEffect->SetRelativeLocation(FVector(0.0f, 100.0f, 45.0f));
	AttackRangeEffect->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	AttackRangeEffect->SetRelativeScale3D(FVector(3.0f, 1.0f, 1.0f));
	AttackRangeEffect->SetHiddenInGame(true);

	ArmLengthTo = 800.0f;
	ArmRotationTo = FRotator(-60.0f, 0.0f, 0.0f);
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

	IsAIControlled = false;

	auto DefaultSetting = GetDefault<USPCharacterSetting>();
	if (DefaultSetting->CharacterAssets.Num() > 0)
	{
		for (auto CharacterAsset : DefaultSetting->CharacterAssets)
		{
			UE_LOG(LogTemp, Warning, TEXT("Character Asset : %s"), *CharacterAsset.ToString());
		}
	}

	AssetIndex = 4;
	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	SetCanBeDamaged(false);

	DeadTimer = 2.0f;
}


void ASPCharacter::SetCharacterState(ECharacterState NewState)
{
	if (CurrentState == NewState) return;
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		if (bIsPlayer)
		{
			DisableInput(SPPlayerController);
		}
		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		SetCanBeDamaged(false);
		break;
	}
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		SetCanBeDamaged(true);

		CharacterStat->OnHPIsZero.AddLambda([this]()->void {
			SetCharacterState(ECharacterState::DEAD);
			});

		//위젯 관련 코드
		auto CharacterWidget = Cast<USPCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		if(nullptr != CharacterWidget)
		{
			CharacterWidget->BindCharacterStat(CharacterStat);
		}
		
		if (bIsPlayer)
		{
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(SPPlayerController);
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			SPAIController->RunAI();
		}
		break;
	}
	case ECharacterState::DEAD:
	{
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		SPAnim->SetDeadAnim();
		SetCanBeDamaged(false);

		if (bIsPlayer)
		{
			SPPlayerController->ShowResultUI();
			DisableInput(SPPlayerController);
		}
		else
		{
			SPAIController->StopAI();
			GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle,
				FTimerDelegate::CreateLambda([this]()->void {
					if (!bIsPlayer) { Destroy(); } }), DeadTimer, false);
		}

		break;
	}
	}
}

void ASPCharacter::SetHasKey(bool bState)
{
	if (bIsPlayer)
	{
		HasKey = bState;
	}
}

bool ASPCharacter::GetHasKey() const
{
	return HasKey;
}

ECharacterState ASPCharacter::GetCharacterState() const
{
	return CurrentState;
}

float ASPCharacter::GetAttackCoolRatio() const
{
	return CurrentAttackCool / CharacterStat->GetAttackDuration();
}

// Called when the game starts or when spawned
void ASPCharacter::BeginPlay()
{
	Super::BeginPlay();


	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		SPPlayerController = Cast<ASPPlayerController>(GetController());
		if (nullptr == SPPlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("SPPlayerController is null"));
		}
	}
	else
	{
		SPAIController = Cast<ASPAIController>(GetController());
		if (nullptr == SPAIController)
		{
			UE_LOG(LogTemp, Warning, TEXT("SPPlayerController is null"));
		}
	}

	auto DefaultSetting = GetDefault<USPCharacterSetting>();
	if (bIsPlayer)
	{
		AssetIndex = 4;
	}
	else
	{
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}

	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	auto SPGameInstance = Cast<USPGameInstance>(GetGameInstance());

	AssetStreamingHandle = SPGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &ASPCharacter::OnAssetLoadCompleted));
	SetCharacterState(ECharacterState::LOADING);
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

	if(!IsAIControlled || EventInstigator->IsPlayerController())
		CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void ASPCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	CharacterStat->SetCharacterStat(IsPlayerControlled());

	if (IsPlayerControlled())
	{
		IsAIControlled = false;
	}
	else
	{
		IsAIControlled = true;
	}
	GetCharacterMovement()->MaxWalkSpeed = CharacterStat->GetSpeed();
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
	if (IsPlayerControlled() && IsAttackDuration) return;
	CurrentAttackCool = 0.0f;
	GetWorldTimerManager().SetTimer(AttackTimer, this, &ASPCharacter::AttackDurationEnd, CharacterStat->GetAttackDuration(), false);
	SPAnim->PlayAttackMontage();
	AttackRangeEffect->SetHiddenInGame(false);
	IsAttacking = true;
}

void ASPCharacter::AttackCheck()
{
	// 충돌한 물체 정보
	FHitResult HitResult;
	// 충돌한 물체 정보에서 자기 자신을 제외
	FCollisionQueryParams Params(NAME_None, false, this);
	// 채널을 사용해 충돌을 탐지함
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
// 캐릭터가 보는 방향
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	// 캡슐의 중심
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	// 캡슐 높이의 절반
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	// 캡슐의 회전 정보
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	// 그린 디버그 정보가 지속되는 시간
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
			GetWorldTimerManager().SetTimer(AttackCoolTimer, this, &ASPCharacter::AttackCool, 0.5f, true, 0.5f);
		}
	}
}

void ASPCharacter::AttackCool()
{
	CurrentAttackCool += 0.5f;
	OnAttackCool.Broadcast();
	if (CurrentAttackCool >= CharacterStat->GetAttackDuration())
	{
		GetWorldTimerManager().ClearTimer(AttackCoolTimer);
	}
}

void ASPCharacter::AttackDurationEnd()
{
	AttackCool();
	// OnAttackCool.Broadcast();
	// CurrentAttackCool = 0.0f;
	// GetWorldTimerManager().ClearTimer(AttackCoolTimer);
	IsAttackDuration = false;
}

void ASPCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	AttackRangeEffect->SetHiddenInGame(true);
	OnAttackEnd.Broadcast();
}

void ASPCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	if (nullptr != AssetLoaded)
	{
		GetMesh()->SetSkeletalMesh(AssetLoaded);
	}
	SetCharacterState(ECharacterState::READY);
}

