// Fill out your copyright notice in the Description page of Project Settings.


#include "SPGamePlayResultWidget.h"
#include "Components/TextBlock.h"
#include "SPGameState.h"

void USPGamePlayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!CurrentGameState.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Current GameState is nullptr"));
		return;
	}

	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	if (nullptr == Result) return;
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ? 
	TEXT("Mission Complete") : TEXT("Mission Failed")));
}

void USPGamePlayResultWidget::BindGameState(ASPGameState* GameState)
{
	if (nullptr == GameState) {
		UE_LOG(LogTemp, Warning, TEXT("Game State is nullptr"));
		return;
	}
	CurrentGameState = GameState;
}