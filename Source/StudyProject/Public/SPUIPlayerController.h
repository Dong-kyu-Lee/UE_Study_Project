// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPUIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API ASPUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	TSubclassOf<class UUserWidget> UIWidgetClass;

	UPROPERTY()
	class UUserWidget* UIWidgetInstance;
};
