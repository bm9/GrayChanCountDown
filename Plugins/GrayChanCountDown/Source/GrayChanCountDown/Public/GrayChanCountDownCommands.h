// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "GrayChanCountDownStyle.h"

class FGrayChanCountDownCommands : public TCommands<FGrayChanCountDownCommands>
{
public:

	FGrayChanCountDownCommands()
		: TCommands<FGrayChanCountDownCommands>(TEXT("GrayChanCountDown"), NSLOCTEXT("Contexts", "GrayChanCountDown", "GrayChanCountDown Plugin"), NAME_None, FGrayChanCountDownStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};