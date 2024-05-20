// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "SPGamePlayWidget.h"
#include "SPGamePlayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USPGamePlayResultWidget : public USPGamePlayWidget
{
	GENERATED_BODY()
	
public:
	void BindGameState(class ASPGameState* GameState);

protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class ASPGameState> CurrentGameState;
};
