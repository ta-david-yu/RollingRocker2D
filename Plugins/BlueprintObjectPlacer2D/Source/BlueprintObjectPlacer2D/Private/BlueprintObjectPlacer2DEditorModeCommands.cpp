// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintObjectPlacer2DEditorModeCommands.h"
#include "BlueprintObjectPlacer2DEditorMode.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "BlueprintObjectPlacer2DEditorModeCommands"

FBlueprintObjectPlacer2DEditorModeCommands::FBlueprintObjectPlacer2DEditorModeCommands()
	: TCommands<FBlueprintObjectPlacer2DEditorModeCommands>("BlueprintObjectPlacer2DEditorMode",
		NSLOCTEXT("BlueprintObjectPlacer2DEditorMode", "BlueprintObjectPlacer2DEditorModeCommands", "BlueprintObjectPlacer2D Editor Mode"),
		NAME_None,
		FEditorStyle::GetStyleSetName())
{
}

void FBlueprintObjectPlacer2DEditorModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);

	UI_COMMAND(SimpleTool, "Show Actor Info", "Opens message box with info about a clicked actor", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(SimpleTool);

	UI_COMMAND(InteractiveTool, "Measure Distance", "Measures distance between 2 points (click to set origin, shift-click to set end point)", EUserInterfaceActionType::ToggleButton, FInputChord());
	ToolCommands.Add(InteractiveTool);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FBlueprintObjectPlacer2DEditorModeCommands::GetCommands()
{
	return FBlueprintObjectPlacer2DEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
