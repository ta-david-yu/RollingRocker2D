// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintObjectPlacer2DEditorMode.h"
#include "BlueprintObjectPlacer2DEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "BlueprintObjectPlacer2DEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/BlueprintObjectPlacer2DSimpleTool.h"
#include "Tools/BlueprintObjectPlacer2DInteractiveTool.h"

// step 2: register a ToolBuilder in FBlueprintObjectPlacer2DEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "BlueprintObjectPlacer2DEditorMode"

const FEditorModeID UBlueprintObjectPlacer2DEditorMode::EM_BlueprintObjectPlacer2DEditorModeId = TEXT("EM_BlueprintObjectPlacer2DEditorMode");

FString UBlueprintObjectPlacer2DEditorMode::SimpleToolName = TEXT("BlueprintObjectPlacer2D_ActorInfoTool");
FString UBlueprintObjectPlacer2DEditorMode::InteractiveToolName = TEXT("BlueprintObjectPlacer2D_MeasureDistanceTool");


UBlueprintObjectPlacer2DEditorMode::UBlueprintObjectPlacer2DEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UBlueprintObjectPlacer2DEditorMode::EM_BlueprintObjectPlacer2DEditorModeId,
		LOCTEXT("ModeName", "BlueprintObjectPlacer2D"),
		FSlateIcon(),
		true);
}


UBlueprintObjectPlacer2DEditorMode::~UBlueprintObjectPlacer2DEditorMode()
{
}


void UBlueprintObjectPlacer2DEditorMode::ActorSelectionChangeNotify()
{
}

void UBlueprintObjectPlacer2DEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FBlueprintObjectPlacer2DEditorModeCommands& SampleToolCommands = FBlueprintObjectPlacer2DEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UBlueprintObjectPlacer2DSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UBlueprintObjectPlacer2DInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UBlueprintObjectPlacer2DEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FBlueprintObjectPlacer2DEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UBlueprintObjectPlacer2DEditorMode::GetModeCommands() const
{
	return FBlueprintObjectPlacer2DEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
