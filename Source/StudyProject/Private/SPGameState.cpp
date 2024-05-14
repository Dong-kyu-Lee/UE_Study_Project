// Fill out your copyright notice in the Description page of Project Settings.


#include "SPGameState.h"

ASPGameState::ASPGameState()
{
	TotalGameScore = 0;
	GameTime = 50;
	CurrentTime = GameTime;
}

int32 ASPGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void ASPGameState::AddGameScore()
{
	TotalGameScore++;
}

int32 ASPGameState::GetCurrentTime() const
{
	return CurrentTime;
}

void ASPGameState::BeginPlay()
{
	GetWorldTimerManager().SetTimer(GameTimer, this, &ASPGameState::UpdateTimer, 1.0f, true);
	UE_LOG(LogTemp, Warning, TEXT("Begin Play"));
}

void ASPGameState::UpdateTimer()
{
	// UE_LOG(LogTemp, Warning, TEXT("UpdateTimer Tick"));
	// CurrentTime�� ������Ʈ
	CurrentTime--;
	// �ð��� 0���� ������ ���� ����
	if (CurrentTime >= 0)
	{
		OnTimeSecond.Broadcast();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(GameTimer);
	}
}
