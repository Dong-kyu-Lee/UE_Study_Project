// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API ASPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASPPlayerController();

	class USPHUDWidget* GetHUDWidget() const;

	void ChangeInputMode(bool bGameMode = true);

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class USPHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class USPGamePlayWidget> MenuWidgetClass;

private:
	void OnGamePause();

	UPROPERTY()
	class USPHUDWidget* HUDWidget;

	class ASPGameState* CurrentGameState;

	UPROPERTY()
	class USPGamePlayWidget* MenuWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
};
