// Fill out your copyright notice in the Description page of Project Settings.


#include "SPAnimInstance.h"

USPAnimInstance::USPAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void USPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
}

void USPAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}
