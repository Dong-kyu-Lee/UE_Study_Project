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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class USPHUDWidget> HUDWidgetClass;

private:
	UPROPERTY()
	class USPHUDWidget* HUDWidget;

	class ASPGameState* CurrentGameState;
};
