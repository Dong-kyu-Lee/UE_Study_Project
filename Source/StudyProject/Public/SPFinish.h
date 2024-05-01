// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "SPFinish.generated.h"

UCLASS()
class STUDYPROJECT_API ASPFinish : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPFinish();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, Category = Finish)
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = Finish)
	UStaticMeshComponent* Door;

private:
	UFUNCTION()
	void OnCharacterHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
};
