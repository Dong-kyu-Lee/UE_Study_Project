// Fill out your copyright notice in the Description page of Project Settings.


#include "SPUIPlayerController.h"
#include "Blueprint/UserWidget.h"


void ASPUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr == UIWidgetClass) return;

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	if (nullptr == UIWidgetInstance) return;

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}
