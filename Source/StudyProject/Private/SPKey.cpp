// Fill out your copyright notice in the Description page of Project Settings.

#include "SPKey.h"
#include "SPCharacter.h"

// Sets default values
ASPKey::ASPKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Key = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KEY"));

	RootComponent = Trigger;
	Key->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_KEY(TEXT("/Game/InfinityBladeWeapons/Weapons/Staff/StaticMesh/SM_Stf_StaffofAncients.SM_Stf_StaffofAncients"));
	if (SM_KEY.Succeeded())
	{
		Key->SetStaticMesh(SM_KEY.Object);
	}
	Key->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Key->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	Trigger->SetCollisionProfileName(TEXT("Item"));
	Key->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void ASPKey::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPKey::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASPKey::OnCharacterOverlap);
}

void ASPKey::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASPCharacter* SPCharacter = Cast<ASPCharacter>(OtherActor);
	if (nullptr != SPCharacter)
	{
		if (SPCharacter->IsPlayerControlled())
		{
			UE_LOG(LogTemp, Warning, TEXT("Collision Occured in Key"));
			SPCharacter->SetHasKey(true);
			Destroy();
		}
	}
}

