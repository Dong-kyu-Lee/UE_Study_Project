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
	Speed = 600.0f;
	CurrentHP = MaxHP;
	Force = 40.0f;
	AttackDuration = 7.0f;
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
	SetHP(MaxHP);
}

void USPCharacterStatComponent::SetDamage(float NewDamage)
{
	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, MaxHP);
	SetHP(CurrentHP);
	if (CurrentHP <= 0.0f)
	{
		OnHPIsZero.Broadcast();
	}
}

float USPCharacterStatComponent::GetAttack()
{
	return Force;
}

void USPCharacterStatComponent::SetCharacterStat(bool IsPlayer)
{
	if (IsPlayer)
	{
		MaxHP = 120.0f;
		CurrentHP = MaxHP;
		Force = 40.0f;
		AttackDuration = 7.0f;
		Speed = 600.0f;
	}
	else
	{
		MaxHP = 80.0f;
		CurrentHP = MaxHP;
		Force = 40.0f;
		Speed = 400.0f;
	}
}

void USPCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	// KINDA_SMALL_NUMBER : 무시할만 한 오차. float를 0과 비교할 때 주로 씀.
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

float USPCharacterStatComponent::GetHPRatio()
{
	return (MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / MaxHP);
}
