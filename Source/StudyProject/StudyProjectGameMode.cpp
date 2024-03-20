// Copyright Epic Games, Inc. All Rights Reserved.

#include "StudyProjectGameMode.h"
#include "StudyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AStudyProjectGameMode::AStudyProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
