// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USPHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindAttackCool(class ASPCharacter* Character);
	void BindLeftTime(class ASPGameState* GameState);

protected:
	virtual void NativeConstruct() override;
	void UpdateTimerText();
	void UpdateAttackCool();

private:
	TWeakObjectPtr<class ASPCharacter> CurrentCharacter;
	TWeakObjectPtr<class ASPGameState> CurrentGameState;

	UPROPERTY()
	class UProgressBar* AttackCoolBar;

	UPROPERTY()
	class UTextBlock* LeftTime;
};
