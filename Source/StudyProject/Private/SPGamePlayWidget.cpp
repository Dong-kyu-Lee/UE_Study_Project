// Fill out your copyright notice in the Description page of Project Settings.


#include "SPGamePlayWidget.h"
#include "Components/Button.h"
#include "SPPlayerController.h"

void USPGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &USPGamePlayWidget::OnResumeClicked);
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &USPGamePlayWidget::OnReturnToTitleClicked);
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &USPGamePlayWidget::OnRetryGameClicked);
	}
}

void USPGamePlayWidget::OnResumeClicked()
{
	auto SPPlayerController = Cast<ASPPlayerController>(GetOwningPlayer());
	if (nullptr == SPPlayerController) return;

	RemoveFromParent();
	SPPlayerController->ChangeInputMode(true);
	SPPlayerController->SetPause(false);
}

void USPGamePlayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void USPGamePlayWidget::OnRetryGameClicked()
{
}
