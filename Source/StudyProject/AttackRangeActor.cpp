// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackRangeActor.h"

// Sets default values
AAttackRangeActor::AAttackRangeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(TEXT("/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule"));
	if (MESH.Succeeded())
	{
		Mesh->SetStaticMesh(MESH.Object);
	}
}

// Called when the game starts or when spawned
void AAttackRangeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

