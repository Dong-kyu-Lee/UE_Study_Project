// Fill out your copyright notice in the Description page of Project Settings.


#include "SPCharacterWidget.h"
#include "SPCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void USPCharacterWidget::BindCharacterStat(class USPCharacterStatComponent* NewCharacterStat)
{
	if (nullptr == NewCharacterStat)
	{
		UE_LOG(LogTemp, Error, TEXT("NewCharacterStat is null"));
		return;
	}

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &USPCharacterWidget::UpdateHPWidget);
}

void USPCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	if(nullptr != HPProgressBar)
	{
		UpdateHPWidget();
	}
}

void USPCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
