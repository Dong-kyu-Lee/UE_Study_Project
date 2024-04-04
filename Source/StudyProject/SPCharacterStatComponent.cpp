// Fill out your copyright notice in the Description page of Project Settings.


#include "SPCharacterStatComponent.h"

// Sets default values for this component's properties
USPCharacterStatComponent::USPCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	
	MaxHP = 120.0f;
	CurrentHP = MaxHP;
	Force = 40.0f;
	AttackDuration = 20.0f;
}

// Called when the game starts
void USPCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USPCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void USPCharacterStatComponent::SetDamage(float NewDamage)
{
	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, MaxHP);
	if (CurrentHP <= 0.0f)
	{
		OnHPIsZero.Broadcast();
	}
}

float USPCharacterStatComponent::GetAttack()
{
	return Force;
}
