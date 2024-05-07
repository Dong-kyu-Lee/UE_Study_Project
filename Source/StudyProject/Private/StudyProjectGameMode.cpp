// Copyright Epic Games, Inc. All Rights Reserved.

#include "StudyProjectGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "SPCharacter.h"
#include "SPPlayerController.h"
#include "SPGameState.h"


AStudyProjectGameMode::AStudyProjectGameMode()
{
	DefaultPawnClass = ASPCharacter::StaticClass();
	PlayerControllerClass = ASPPlayerController::StaticClass();
	GameStateClass = ASPGameState::StaticClass();
}
