// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#include "AssetBox.h"
#include "Helpers.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"
#include "StaticMeshAsset.h"
#include "MeshAsset.h"
#include "StaticMeshAsset.h"
#include "Asset.h"


void UAssetBox::Init(UAsset* InAsset)
{
    Asset = InAsset;

	// Add checkbox
	LoadButton = NewObject<UCheckBox>();
	LoadButton->OnCheckStateChanged.AddDynamic(this, &UAssetBox::OnStateChanged);

	UHorizontalBoxSlot* UISlot = AddChildToHorizontalBox(LoadButton);
	UISlot->SetPadding(FMargin(40, 5));

	// Create text
	FLinearColor White = FLinearColor(1, 1, 1);
	ContentNameText = UHelpers::MakeTextBlock(Asset->CutName, 10, White);
	SceneNameText = UHelpers::MakeTextBlock(Asset->SceneName, 10, White);
	ObjectTypeText = UHelpers::MakeTextBlock(Asset->Type, 10, White);

	// Add text to GUI
	USizeBox* Size = NewObject<USizeBox>();
	Size->SetWidthOverride(150);
	Size->SetContent(ContentNameText);
	UISlot = AddChildToHorizontalBox(Size);

	Size = NewObject<USizeBox>();
	Size->SetWidthOverride(150);
	Size->SetContent(SceneNameText);
	UISlot = AddChildToHorizontalBox(Size);

	Size = NewObject<USizeBox>();
	Size->SetWidthOverride(150);
	Size->SetContent(ObjectTypeText);
	UISlot = AddChildToHorizontalBox(Size);

	// Update GUI based on scene state
	EvaluateState();
}



void UAssetBox::OnStateChanged(bool NewState)
{
	// Check if object loaded in the scene
	bool loaded = Asset->IsLoaded();

	// Make sure new state is different before changing
	bool Changed = false;
	if (NewState == true && loaded == false)
	{

		Asset->Load();
		Changed = true;
	}
	else if (NewState == false && loaded == true)
	{

		Asset->Unload();
		Changed = true;

	}
	// If change occured, update sequencer and GUI
	if (Changed)
	{
		UHelpers::UpdateSequencer(Asset->Sequencer);
		EvaluateState();
	}
}


void UAssetBox::EvaluateState()
{
	// Check if object loaded in scene
	bool loaded = Asset->IsLoaded();

	// Update checkbox, and text color based on state
	if (loaded)
	{
		LoadButton->SetIsChecked(true);
		ContentNameText->SetColorAndOpacity(FLinearColor(0, 1, 0));
		SceneNameText->SetColorAndOpacity(FLinearColor(0, 1, 0));
		ObjectTypeText->SetColorAndOpacity(FLinearColor(0, 1, 0));

	}
	else
	{
		LoadButton->SetIsChecked(false);
		ContentNameText->SetColorAndOpacity(FLinearColor(0, 0, 0));
		SceneNameText->SetColorAndOpacity(FLinearColor(0, 0, 0));
		ObjectTypeText->SetColorAndOpacity(FLinearColor(0, 0, 0));

	}
}

