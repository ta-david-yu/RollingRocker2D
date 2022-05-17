// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintObjectPlacer2DEditorModeToolkit.h"
#include "BlueprintObjectPlacer2DEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "BlueprintObjectPlacer2DEditorModeToolkit"

FBlueprintObjectPlacer2DEditorModeToolkit::FBlueprintObjectPlacer2DEditorModeToolkit()
{
}

void FBlueprintObjectPlacer2DEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FBlueprintObjectPlacer2DEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FBlueprintObjectPlacer2DEditorModeToolkit::GetToolkitFName() const
{
	return FName("BlueprintObjectPlacer2DEditorMode");
}

FText FBlueprintObjectPlacer2DEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "BlueprintObjectPlacer2DEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
