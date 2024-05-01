// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Components/ActorComponent.h"
#include "SPCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STUDYPROJECT_API USPCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USPCharacterStatComponent();

	UFUNCTION(BlueprintCallable, Category = Stat)
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	float GetAttack();
	float GetSpeed() const { return Speed; }
	float GetHPRatio();
	float GetAttackDuration() const { return AttackDuration; }
	void SetCharacterStat(bool IsPlayer);

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

private:
	float MaxHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(Transient, VisibleInstanceOnly, Category=Stat, Meta=(AllowPrivateAccess = true))
	float CurrentHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stat, Meta=(AllowPrivateAccess = true))
	float Force;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackDuration;
};