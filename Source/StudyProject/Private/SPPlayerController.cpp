// Fill out your copyright notice in the Description page of Project Settings.


#include "SPPlayerController.h"
#include "SPHUDWidget.h"
#include "SPGameState.h"
#include "SPCharacter.h"
#include "SPGamePlayWidget.h"

ASPPlayerController::ASPPlayerController()
{
	static ConstructorHelpers::FClassFinder<USPHUDWidget> UI_HUD_C(
		TEXT("/Game/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<USPGamePlayWidget> UI_MENU_C(
		TEXT("/Game/UI/UI_Menu.UI_Menu_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}
}

USPHUDWidget* ASPPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void ASPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this,
		&ASPPlayerController::OnGamePause);
}

void ASPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ChangeInputMode(true);

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDWidget = CreateWidget<USPHUDWidget>(this, HUDWidgetClass);
	if (nullptr == HUDWidget) return;
	HUDWidget->AddToViewport(1);

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

void ASPPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void ASPPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<USPGamePlayWidget>(this, MenuWidgetClass);
	if (nullptr == MenuWidget) return;
	MenuWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}
