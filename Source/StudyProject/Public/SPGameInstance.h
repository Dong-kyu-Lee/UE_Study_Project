// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "SPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USPGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USPGameInstance();

	virtual void Init() override;

	FStreamableManager StreamableManager;
};
