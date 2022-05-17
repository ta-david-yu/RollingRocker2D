// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintObjectPlacer2DModule.h"
#include "BlueprintObjectPlacer2DEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "BlueprintObjectPlacer2DModule"

void FBlueprintObjectPlacer2DModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FBlueprintObjectPlacer2DEditorModeCommands::Register();
}

void FBlueprintObjectPlacer2DModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FBlueprintObjectPlacer2DEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBlueprintObjectPlacer2DModule, BlueprintObjectPlacer2DEditorMode)