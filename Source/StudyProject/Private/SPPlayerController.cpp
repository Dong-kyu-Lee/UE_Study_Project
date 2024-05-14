// Fill out your copyright notice in the Description page of Project Settings.


#include "SPPlayerController.h"
#include "SPHUDWidget.h"
#include "SPGameState.h"
#include "SPCharacter.h"

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

	auto SPGameState = Cast<ASPGameState>(GetWorld()->GetGameState());
	if (SPGameState == nullptr){
		UE_LOG(LogTemp, Warning, TEXT("SPGameState is nullptr"));
		return;
	}
	auto SPCharacter = Cast<ASPCharacter>(GetCharacter());
	if (SPCharacter == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("SPCharacter is nullptr"));
		return;
	}
	HUDWidget->BindLeftTime(SPGameState);
	HUDWidget->BindAttackCool(SPCharacter);
}