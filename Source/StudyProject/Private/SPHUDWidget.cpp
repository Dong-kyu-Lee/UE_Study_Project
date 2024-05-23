// Fill out your copyright notice in the Description page of Project Settings.


#include "SPHUDWidget.h"
#include "SPCharacter.h"
#include "SPGameState.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USPHUDWidget::BindAttackCool(ASPCharacter* Character)
{
	CurrentCharacter = Character;
	CurrentCharacter->OnAttackCool.AddUObject(this, &USPHUDWidget::UpdateAttackCool);
}

// Player Controller에서 호출
void USPHUDWidget::BindLeftTime(ASPGameState* GameState)
{
	CurrentGameState = GameState;
	CurrentGameState->OnTimeSecond.AddUObject(this, &USPHUDWidget::UpdateTimerText);
}

void USPHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AttackCoolBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("AttackCool")));
	if (AttackCoolBar == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("AttackCool Bar is nullptr"));
		return;
	}
	LeftTime = Cast<UTextBlock>(GetWidgetFromName(TEXT("LeftTime")));
	if (LeftTime == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("LeftTime Text is nullptr"));
		return;
	}

	AttackCoolBar->SetPercent(1.0f);
}

void USPHUDWidget::UpdateAttackCool()
{
	if (CurrentCharacter.IsValid())
	{
		AttackCoolBar->SetPercent(CurrentCharacter->GetAttackCoolRatio());
	}
}

void USPHUDWidget::UpdateTimerText()
{
#define LOCTEXT_NAMESPACE "MyNamespace"
	int32 CurrentTime = CurrentGameState->GetCurrentTime();
	int32 Min, Sec;
	Min = CurrentTime / 60;
	Sec = CurrentTime % 60;
	if (Sec >= 10)
		LeftTime->SetText(FText::Format(LOCTEXT("ExampleFText", "{0}:{1}"), Min, Sec));
	else
		LeftTime->SetText(FText::Format(LOCTEXT("ExampleFText", "{0}:0{1}"), Min, Sec));
#undef LOCTEXT_NAMESPACE
}
