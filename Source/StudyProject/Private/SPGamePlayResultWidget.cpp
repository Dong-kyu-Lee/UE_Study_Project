// Fill out your copyright notice in the Description page of Project Settings.


#include "SPGamePlayResultWidget.h"
#include "Components/TextBlock.h"

void USPGamePlayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	if (nullptr == Result) return;

	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	if (nullptr == TotalScore) {
		UE_LOG(LogTemp, Warning, TEXT("TotalScore is nullptr"));
		return;
	}
}
