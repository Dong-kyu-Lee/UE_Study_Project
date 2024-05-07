// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SPGameState.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API ASPGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ASPGameState();

public:
	int32 GetTotalGameScore() const;
	void AddGameScore();

private:
	UPROPERTY(Transient)
	int32 TotalGameScore;
};
