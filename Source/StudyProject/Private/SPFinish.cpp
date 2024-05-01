// Fill out your copyright notice in the Description page of Project Settings.


#include "SPFinish.h"
#include "SPCharacter.h"

// Sets default values
ASPFinish::ASPFinish()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DOOR"));

	RootComponent = Trigger;
	Door->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(10.0f, 42.0f, 80.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_DOOR(TEXT("/Game/StarterContent/Props/SM_Door.SM_Door"));
	if (SM_DOOR.Succeeded())
	{
		Door->SetStaticMesh(SM_DOOR.Object);
	}
	Door->SetRelativeScale3D(FVector(1.0f, 1.2f, 1.1f));
	Door->SetRelativeLocation(FVector(0.0f, 50.0f, -60.0f));
	Door->SetCollisionProfileName(TEXT("NoCollision"));
	Trigger->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

// Called when the game starts or when spawned
void ASPFinish::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentHit.AddDynamic(this, &ASPFinish::OnCharacterHit);
}

void ASPFinish::OnCharacterHit(UPrimitiveComponent* HitComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ASPCharacter* SPCharacter = Cast<ASPCharacter>(OtherActor);
	if (nullptr != SPCharacter)
	{
		if (SPCharacter->IsPlayerControlled() && SPCharacter->GetHasKey())
		{
			UE_LOG(LogTemp, Warning, TEXT("Door Opened!"));
			SPCharacter->SetHasKey(false);
			Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Door->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		}
	}
}
