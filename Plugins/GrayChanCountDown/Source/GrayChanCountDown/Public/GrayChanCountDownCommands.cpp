// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GrayChanCountDownPrivatePCH.h"
#include "GrayChanCountDownCommands.h"

#define LOCTEXT_NAMESPACE "FGrayChanCountDownModule"

void FGrayChanCountDownCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "GrayChanCD", "Bring up GrayChanCountDown window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
