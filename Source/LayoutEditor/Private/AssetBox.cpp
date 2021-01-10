// Fill out your copyright notice in the Description page of Project Settings.


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
	//LoadButton::OnCheckStateChanged()
	LoadButton = NewObject<UCheckBox>(); //SNew(SCheckBox).OnCheckStateChanged(this, &UAssetBox::OnStateChanged);

	FLinearColor White = FLinearColor(1, 1, 1);
	ContentNameText = UHelpers::MakeTextBlock(Asset->CutName, 10, White);
	SceneNameText = UHelpers::MakeTextBlock(Asset->SceneName, 10, White);
	ObjectTypeText = UHelpers::MakeTextBlock(Asset->Type, 10, White);

	UHorizontalBoxSlot *UISlot = AddChildToHorizontalBox(LoadButton);
	UISlot->SetPadding(FMargin(40, 5));

	USizeBox *Size = NewObject<USizeBox>();
	Size->SetWidthOverride(150);
	Size->SetContent(ContentNameText);
	UISlot = AddChildToHorizontalBox(Size);
	//UISlot->SetPadding(FMargin(20, 5));

	Size = NewObject<USizeBox>();
	Size->SetWidthOverride(150);
	Size->SetContent(SceneNameText);
	UISlot = AddChildToHorizontalBox(Size);

	Size = NewObject<USizeBox>();
	Size->SetWidthOverride(150);
	Size->SetContent(ObjectTypeText);
	UISlot = AddChildToHorizontalBox(Size);
	//UISlot->SetPadding(FMargin(20, 5));

	EvaluateState();
	LoadButton->OnCheckStateChanged.AddDynamic(this, &UAssetBox::OnStateChanged);


}



void UAssetBox::OnStateChanged(bool NewState)
{

	bool loaded = Asset->IsLoaded();

	bool Changed = false;

	if (NewState == true && loaded == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading"));

		Asset->Load();
		Changed = true;
	}
	else if (NewState == false && loaded == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("UnLoading"));

		Asset->Unload();
		Changed = true;

	}
	if (Changed)
	{
		UHelpers::UpdateSequencer(Asset->Sequencer);
		EvaluateState();
	}

}




void UAssetBox::EvaluateState()
{


	bool loaded = Asset->IsLoaded();
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

