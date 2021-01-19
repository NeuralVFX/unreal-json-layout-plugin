// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#include "LayoutWidget.h"
#include "Engine/Selection.h"
#include "Asset.h"
#include "MeshAsset.h"
#include "StaticMeshAsset.h"
#include "AnimatedMeshAsset.h"
#include "AssetBox.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequence.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Components/PanelWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"
#include "HAL/FileManager.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Toolkits/AssetEditorManager.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "ISequencer.h"
#include "Helpers.h"
#include "JsonLayout.h"


ULayoutWidget::ULayoutWidget()
{
}


ULayoutWidget::~ULayoutWidget()
{
}


void ULayoutWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	UCanvasPanel* Root = Cast<UCanvasPanel>(GetRootWidget());

	// Setup scroll box to hold layout assets
	UScrollBox* ScrollBox = NewObject<UScrollBox>();
	VerticalBox = NewObject<UVerticalBox>();
	ScrollBox->AddChild(VerticalBox);

	UBorder* Border =NewObject<UBorder>();
	Border->SetContent(ScrollBox);
	Border->SetBrushColor(FLinearColor(.1, .1, .1));
	
	// Add scroll box to GUI
	UCanvasPanelSlot* UISlot = Root->AddChildToCanvas(Border);
	UISlot->SetSize(FVector2D(550, 400));
	UISlot->SetPosition(FVector2D(10, 30));

	// Setup horizontal box to hold GUI buttons
	UHorizontalBox * BotBox = NewObject<UHorizontalBox>();
	UBorder* BotBorder = NewObject<UBorder>();
	BotBorder->SetContent(BotBox);
	BotBorder->SetBrushColor(FLinearColor(.1, .1, .1));

	// Add button area to GUI
	UISlot = Root->AddChildToCanvas(BotBorder);
	UISlot->SetSize(FVector2D(550, 30));
	UISlot->SetPosition(FVector2D(10, 440));

	// Setup buttons
	UButton*  LoadAllButton = NewObject<UButton>();
	LoadAllButton->OnClicked.AddDynamic(this, &ULayoutWidget::LoadAll);
	UHorizontalBoxSlot* UISlot_Hor = BotBox->AddChildToHorizontalBox(LoadAllButton);
	UISlot_Hor->SetPadding(FMargin(13, 2));

	UButton* UnLoadAllButton = NewObject<UButton>();
	UnLoadAllButton->OnClicked.AddDynamic(this, &ULayoutWidget::UnLoadAll);
	UISlot_Hor = BotBox->AddChildToHorizontalBox(UnLoadAllButton);
	UISlot_Hor->SetPadding(FMargin(13, 2));

	UButton* LoadJsonButton = NewObject<UButton>();
	LoadJsonButton->OnClicked.AddDynamic(this, &ULayoutWidget::LoadJson);
	UISlot_Hor = BotBox->AddChildToHorizontalBox(LoadJsonButton);
	UISlot_Hor->SetPadding(FMargin(13, 2));

	UButton* SaveJsonButton = NewObject<UButton>();
	SaveJsonButton->OnClicked.AddDynamic(this, &ULayoutWidget::SaveJson);
	UISlot_Hor = BotBox->AddChildToHorizontalBox(SaveJsonButton);
	UISlot_Hor->SetPadding(FMargin(13, 2));

	// Setup text
	FLinearColor Black = FLinearColor(0, 0,0 );
	FLinearColor White = FLinearColor(1, 1, 1);

	UTextBlock* LoadAllText = UHelpers::MakeTextBlock("Load All Assets", 10, Black);
	UTextBlock* UnLoadAllText = UHelpers::MakeTextBlock("UnLoad All Assets", 10, Black);
	UTextBlock* IsLoadedText = UHelpers::MakeTextBlock("Is Loaded", 10, White);
	UTextBlock* SceneNameText = UHelpers::MakeTextBlock("Scene Name", 10, White);
	UTextBlock* ContentNameText = UHelpers::MakeTextBlock("Content Name", 10,White);
	UTextBlock* ObjectTypeText = UHelpers::MakeTextBlock("Object Type", 10, White);
	UTextBlock* LoadJsonText = UHelpers::MakeTextBlock("Load Layout File", 10, Black);
	UTextBlock* SaveJsonText = UHelpers::MakeTextBlock("Save Layout File", 10, Black);

	// Set all button text
	LoadAllButton->SetContent(LoadAllText);
	UnLoadAllButton->SetContent(UnLoadAllText);
	SaveJsonButton->SetContent(SaveJsonText);
	LoadJsonButton->SetContent(LoadJsonText);

	// Setup column label arrea
	UHorizontalBox* TopBox=  NewObject<UHorizontalBox>();

	USizeBox* Size = NewObject<USizeBox>();
	Size->SetWidthOverride(70);
	Size->SetContent(IsLoadedText);
	UISlot_Hor = TopBox->AddChildToHorizontalBox(Size);
	UISlot_Hor->SetPadding(FMargin(15, 0));

	// Add text to column label area
	Size = NewObject<USizeBox>();
	Size->SetWidthOverride(150);
	Size->SetContent(ContentNameText);
	UISlot_Hor = TopBox->AddChildToHorizontalBox(Size);

	Size = NewObject<USizeBox>();
	Size->SetWidthOverride(150);
	Size->SetContent(SceneNameText);
	UISlot_Hor = TopBox->AddChildToHorizontalBox(Size);

	Size = NewObject<USizeBox>();
	Size->SetWidthOverride(150);
	Size->SetContent(ObjectTypeText);
	UISlot_Hor = TopBox->AddChildToHorizontalBox(Size);

	UBorder* TopBorder = NewObject<UBorder>();
	TopBorder->SetContent(TopBox);
	TopBorder->SetBrushColor(FLinearColor(.1, .1, .1));

	// Add column label area to GUI
	UISlot = Root->AddChildToCanvas(TopBorder);
	UISlot->SetSize(FVector2D(550, 20));
	UISlot->SetPosition(FVector2D(10, 5));
}


void ULayoutWidget::SaveJson()
{
	// Get all secected actors
	TArray<AStaticMeshActor*> ActorArray;
	USelection* SelectedActors = GEditor->GetSelectedActors();

	// Loop through actors, add to array if StaticMeshActor
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AActor* Actor = Cast<AActor>(*Iter);
		if (Actor)
		{
			AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(Actor);
			if (MeshActor)
			{
				ActorArray.Add(MeshActor);
			}
		}
	}
	
	// If actors found, write JSON file
	if (ActorArray.Num() > 0)
	{
		FString OutFile = SaveJsonFile();
		UJsonLayout::WriteLayoutData(OutFile, ActorArray, GetSequenceActor());
	}


}


void ULayoutWidget::LoadJson()
{
	// Clear assets from GUI
	VerticalBox->ClearChildren();
	AssetBoxArray.Empty();

	// Get Sequencer
	ALevelSequenceActor* CurrentSequencer = GetSequenceActor();
	
	// Open JSON file
	FString FileName = LoadJsonFile();
	FStaticMeshArray Objects =  UJsonLayout::ReadLayoutData(FileName);

	// Loop through StaticMesh array, create asset, add to GUI
	for  (FStaticMeshObject Struct:Objects.StaticMeshes)
	{ 
		// Get details from struct
		FString NewSceneName = Struct.SceneName;
		FString NewContentName = Struct.ContentName;
		FMatrix Matrix = Struct.Matrix;
		FTransform NewTransform(Matrix);

		// Create asset
		UStaticMeshAsset* NewAsset = NewObject<UStaticMeshAsset>();
		NewAsset->Init(CurrentSequencer, NewSceneName, NewContentName, NewTransform);
		NewAsset->AddToRoot();

		// Create asset box for GUI
		UAssetBox* AssetBoxA = NewObject<UAssetBox>();
		AssetBoxA->Init(Cast<UAsset>(NewAsset));

		// Add asset box to GUI
		VerticalBox->AddChildToVerticalBox(AssetBoxA);
		AssetBoxArray.Add(AssetBoxA);
	}


	// Loop through animated StaticMesh array, create asset, add to GUI
	for (FAnimatedStaticMeshObject Struct : Objects.AnimatedStaticMeshes)
	{
		// Get details from struct
		FString NewSceneName = Struct.SceneName;
		FString NewContentName = Struct.ContentName;

		// Create asset
		UAnimatedMeshAsset* NewAsset = NewObject<UAnimatedMeshAsset>();
		NewAsset->Init(CurrentSequencer, NewSceneName, NewContentName, Struct);
		NewAsset->AddToRoot();

		// Create asset box for GUI
		UAssetBox* AssetBoxA = NewObject<UAssetBox>();
		AssetBoxA->Init(Cast<UAsset>(NewAsset));

		// Add asset box to GUI
		VerticalBox->AddChildToVerticalBox(AssetBoxA);
		AssetBoxArray.Add(AssetBoxA);
	}
}



void ULayoutWidget::LoadAll()
{
	// Attempt to load each asset, update GUI
	for (UAssetBox* AssetBoxA : AssetBoxArray)
	{
		AssetBoxA->Asset->Load();
		AssetBoxA->EvaluateState();
	}

	// Update sequecer GUI
	ALevelSequenceActor* CurrentSequencer = GetSequenceActor();
	UHelpers::UpdateSequencer(CurrentSequencer);
}


void ULayoutWidget::UnLoadAll()
{
	// Attempt to unload each asset
	for (UAssetBox* AssetBoxA : AssetBoxArray)
	{
		AssetBoxA->Asset->Unload();
		AssetBoxA->EvaluateState();
	}

	// Update sequecer GUI
	ALevelSequenceActor* CurrentSequencer = GetSequenceActor();
	UHelpers::UpdateSequencer(CurrentSequencer);
}


FString ULayoutWidget::LoadJsonFile()
{
	TArray<FString> OutFileNames;
	FString OutName;

	// Setup GUI related arguments
	const void* ParentWindowPtr = UHelpers::GetParentWindow();
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	// Let user select file from browser GUI
	if (DesktopPlatform)
	{
		uint32 SelectionFlag = 1; 
		DesktopPlatform->OpenFileDialog(ParentWindowPtr,
			FString("Load Json"),
			FPaths::ProjectSavedDir() / "LayoutJson",
			FString(""),
			TEXT("(Layout Files)|*.json;)"),
			SelectionFlag,
			OutFileNames);
	}
	
	// Fetch output
	if (OutFileNames.Num() > 0)
	{
		OutName = OutFileNames.Pop();
	}

	return OutName;
}


FString ULayoutWidget::SaveJsonFile()
{
	TArray<FString> OutFileNames;
	FString OutName;

	// Setup GUI related arguments
	const void* ParentWindowPtr = UHelpers::GetParentWindow();
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	// Let user create or select file from browser GUI
	if (DesktopPlatform)
	{
		uint32 SelectionFlag = 1;
		DesktopPlatform->SaveFileDialog(ParentWindowPtr,
			FString("Save Json"),
			FPaths::ProjectSavedDir() / "LayoutJson",
			FString(""),
			TEXT("(Layout Files)|*.json;)"),
			SelectionFlag,
			OutFileNames);
	}

	// Fetch output
	if (OutFileNames.Num() > 0)
	{
		OutName = OutFileNames.Pop();
	}

	return OutName;
}


ALevelSequenceActor* ULayoutWidget::GetSequenceActor()
{
	// Get list of all LevelSequenceActors
	ALevelSequenceActor* CurrentSequencer = nullptr;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),
		ALevelSequenceActor::StaticClass(),
		FoundActors);

	// Return first one found
	if (FoundActors.Num() > 0)
	{
		CurrentSequencer = Cast<ALevelSequenceActor>(FoundActors[0]);
	}

	return CurrentSequencer;
}
