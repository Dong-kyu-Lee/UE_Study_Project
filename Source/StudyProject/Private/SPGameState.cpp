// Fill out your copyright notice in the Description page of Project Settings.


#include "SPGameState.h"

ASPGameState::ASPGameState()
{
	TotalGameScore = 0;
}

int32 ASPGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void ASPGameState::AddGameScore()
{
	TotalGameScore++;
}
