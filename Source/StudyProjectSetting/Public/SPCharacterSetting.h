// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SPCharacterSetting.generated.h"

/**
 * 
 */
UCLASS(config=StudyProject)
class STUDYPROJECTSETTING_API USPCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	USPCharacterSetting();

	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;
};
