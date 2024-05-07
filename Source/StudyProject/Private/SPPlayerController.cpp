// Fill out your copyright notice in the Description page of Project Settings.


#include "SPPlayerController.h"
#include "SPHUDWidget.h"

ASPPlayerController::ASPPlayerController()
{
	static ConstructorHelpers::FClassFinder<USPHUDWidget> UI_HUD_C(
		TEXT("/Game/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}
}

USPHUDWidget* ASPPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void ASPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDWidget = CreateWidget<USPHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();
}