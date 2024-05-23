// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SPGameState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnTimeSecondDelegate);
DECLARE_MULTICAST_DELEGATE(FOnTimeUpDelegate);

UCLASS()
class STUDYPROJECT_API ASPGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ASPGameState();

public:
	int32 GetTotalGameScore() const;
	void AddGameScore();
	int32 GetCurrentTime() const;
	void SetGameCleared();
	bool IsGameCleared() const;

	FOnTimeSecondDelegate OnTimeSecond;
	FOnTimeUpDelegate OnTimeUp;

protected:
	virtual void BeginPlay() override;

private:
	void UpdateTimer();

	UPROPERTY(Transient)
	bool bGameCleared;

	UPROPERTY(Transient)
	int32 TotalGameScore;

	UPROPERTY()
	int32 GameTime;

	int32 CurrentTime;

	FTimerHandle GameTimer;
};
