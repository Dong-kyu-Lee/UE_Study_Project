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
	
protected:
	virtual void NativeConstruct() override;
};
