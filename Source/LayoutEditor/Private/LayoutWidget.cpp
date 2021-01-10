// Fill out your copyright notice in the Description page of Project Settings.
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
#include "C:/Program Files/Epic Games/UE_4.26/Engine/Plugins/MovieScene/LevelSequenceEditor/Source/LevelSequenceEditor/Private/LevelSequenceEditorToolkit.h"
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

	ALevelSequenceActor* CurrentSequencer = GetSequenceActor();
	//UHelpers::UpdateSequencer(CurrentSequencer);

	UCanvasPanel* root = Cast<UCanvasPanel>(GetRootWidget());

	// Setup Asset Box
	UScrollBox* ScrollBox = NewObject<UScrollBox>();
	VerticalBox = NewObject<UVerticalBox>();
	ScrollBox->AddChild(VerticalBox);

	UBorder *Border =NewObject<UBorder>();
	Border->SetContent(ScrollBox);
	Border->SetBrushColor(FLinearColor(.1, .1, .1));

	UCanvasPanelSlot *UISlot = root->AddChildToCanvas(Border);
	UISlot->SetSize(FVector2D(550, 400));
	UISlot->SetPosition(FVector2D(10, 30));

	UHorizontalBox * BotBox = NewObject<UHorizontalBox>();
	UBorder *BotBorder = NewObject<UBorder>();
	BotBorder->SetContent(BotBox);
	BotBorder->SetBrushColor(FLinearColor(.1, .1, .1));

	UISlot = root->AddChildToCanvas(BotBorder);
	UISlot->SetSize(FVector2D(550, 30));
	UISlot->SetPosition(FVector2D(10, 440));

	UButton*  LoadAllButton = NewObject<UButton>();
	LoadAllButton->OnClicked.AddDynamic(this, &ULayoutWidget::LoadAll);
	UHorizontalBoxSlot *UISlot_Hor = BotBox->AddChildToHorizontalBox(LoadAllButton);
	UISlot_Hor->SetPadding(FMargin(13, 2));
	//UISlot = root->AddChildToCanvas(Cast<UWidget>(LoadAllButton));
	//UISlot->SetPosition(FVector2D(350, 450));

	UButton* UnLoadAllButton = NewObject<UButton>();
	UnLoadAllButton->OnClicked.AddDynamic(this, &ULayoutWidget::UnLoadAll);
	UISlot_Hor = BotBox->AddChildToHorizontalBox(UnLoadAllButton);
	UISlot_Hor->SetPadding(FMargin(13, 2));
	//UISlot = root->AddChildToCanvas(Cast<UWidget>(UnLoadAllButton));
	//UISlot->SetPosition(FVector2D(100, 450));

	UButton* LoadJsonButton = NewObject<UButton>();
	LoadJsonButton->OnClicked.AddDynamic(this, &ULayoutWidget::LoadJson);
	UISlot_Hor = BotBox->AddChildToHorizontalBox(LoadJsonButton);
	UISlot_Hor->SetPadding(FMargin(13, 2));
	//UISlot = root->AddChildToCanvas(Cast<UWidget>(LoadJsonButton));
	//UISlot->SetPosition(FVector2D(550, 450));

	UButton* SaveJsonButton = NewObject<UButton>();
	SaveJsonButton->OnClicked.AddDynamic(this, &ULayoutWidget::SaveJson);
	UISlot_Hor = BotBox->AddChildToHorizontalBox(SaveJsonButton);
	UISlot_Hor->SetPadding(FMargin(13, 2));
	//UISlot = root->AddChildToCanvas(Cast<UWidget>(SaveJsonButton));
	//UISlot->SetPosition(FVector2D(650, 450));

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

	LoadAllButton->SetContent(LoadAllText);
	UnLoadAllButton->SetContent(UnLoadAllText);
	SaveJsonButton->SetContent(SaveJsonText);
	LoadJsonButton->SetContent(LoadJsonText);

	UHorizontalBox * TopBox=  NewObject<UHorizontalBox>();


	USizeBox *Size = NewObject<USizeBox>();
	Size->SetWidthOverride(70);
	Size->SetContent(IsLoadedText);
	UISlot_Hor = TopBox->AddChildToHorizontalBox(Size);
	UISlot_Hor->SetPadding(FMargin(15, 0));

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

	UBorder *TopBorder = NewObject<UBorder>();
	TopBorder->SetContent(TopBox);
	TopBorder->SetBrushColor(FLinearColor(.1, .1, .1));


	UISlot = root->AddChildToCanvas(TopBorder);
	UISlot->SetSize(FVector2D(550, 20));
	UISlot->SetPosition(FVector2D(10, 5));



}



void ULayoutWidget::SaveJson()
{
	
	TArray<AStaticMeshActor*> ActorArray;

	USelection* SelectedActors = GEditor->GetSelectedActors();
	TArray<ULevel*> UniqueLevels;
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
	

	if (ActorArray.Num() > 0)
	{
		FString OutFile = SaveJsonFile();
		UJsonLayout::WriteLayoutData(OutFile, ActorArray, GetSequenceActor());
	}


}





void ULayoutWidget::LoadJson()
{

	VerticalBox->ClearChildren();
	AssetBoxArray.Empty();
	AssetArray.Empty();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelSequenceActor::StaticClass(), FoundActors);
	ALevelSequenceActor* CurrentSequencer = Cast<ALevelSequenceActor>(FoundActors[0]);
	
	

	FString FileName = LoadJsonFile();
	UE_LOG(LogTemp, Warning, TEXT("File %s"), *FileName);

	FStaticMeshArray Objects =  UJsonLayout::ReadLayoutData(FileName);

	for  (FStaticMeshObject Struct:Objects.StaticMeshes)
	{ 
		FString NewSceneName = Struct.SceneName;
		FString NewContentName = Struct.ContentName;
		FMatrix Matrix = Struct.Matrix;
		FTransform NewTransform(Matrix);

		UStaticMeshAsset *AssetB = NewObject<UStaticMeshAsset>();
		AssetB->Init(CurrentSequencer, NewSceneName, NewContentName, NewTransform);
		AssetB->AddToRoot();

		UAssetBox * AssetBoxA = NewObject<UAssetBox>();
		AssetBoxA->Init(Cast<UAsset>(AssetB));

		VerticalBox->AddChildToVerticalBox(AssetBoxA);

		AssetArray.Add(AssetB);
		AssetBoxArray.Add(AssetBoxA);
	}



	for (FAnimatedStaticMeshObject Struct : Objects.AnimatedStaticMeshes)
	{
		FString NewSceneName = Struct.SceneName;
		FString NewContentName = Struct.ContentName;

		UAnimatedMeshAsset *AssetB = NewObject<UAnimatedMeshAsset>();
		AssetB->Init(CurrentSequencer, NewSceneName, NewContentName, Struct);
		AssetB->AddToRoot();

		UAssetBox * AssetBoxA = NewObject<UAssetBox>();
		AssetBoxA->Init(Cast<UAsset>(AssetB));

		VerticalBox->AddChildToVerticalBox(AssetBoxA);

		AssetArray.Add(AssetB);
		AssetBoxArray.Add(AssetBoxA);
	}

}




void ULayoutWidget::LoadAll()
{

	for (UAssetBox *AssetBoxA : AssetBoxArray)
	{
		AssetBoxA->Asset->Load();
		AssetBoxA->EvaluateState();
	}

	ALevelSequenceActor* CurrentSequencer = GetSequenceActor();
	UHelpers::UpdateSequencer(CurrentSequencer);
}


void ULayoutWidget::UnLoadAll()
{

	for (UAssetBox *AssetBoxA : AssetBoxArray)
	{
		AssetBoxA->Asset->Unload();
		AssetBoxA->EvaluateState();
	}

	ALevelSequenceActor* CurrentSequencer = GetSequenceActor();
	UHelpers::UpdateSequencer(CurrentSequencer);
}


FString ULayoutWidget::LoadJsonFile()
{

	TArray<FString> OutFileNames;
	FString OutName;

	const void* ParentWindowPtr = UHelpers::GetParentWindow();
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		uint32 SelectionFlag = 1; 
		DesktopPlatform->OpenFileDialog(ParentWindowPtr, FString("Load Json"),
										FPaths::ProjectSavedDir() / "LayoutJson",
										FString(""),
										TEXT("(Layout Files)|*.json;)"),
										SelectionFlag,
										OutFileNames);
	}
	
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

	const void* ParentWindowPtr = UHelpers::GetParentWindow();
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		uint32 SelectionFlag = 1;
		//
		DesktopPlatform->SaveFileDialog(ParentWindowPtr,
										FString("Save Json"),
										FPaths::ProjectSavedDir() / "LayoutJson",
										FString(""),
										TEXT("(Layout Files)|*.json;)"),
										SelectionFlag,
										OutFileNames);
	}

	if (OutFileNames.Num() > 0)
	{
		OutName = OutFileNames.Pop();
	}
	return OutName;
}


ALevelSequenceActor* ULayoutWidget::GetSequenceActor()
{
	ALevelSequenceActor* CurrentSequencer = nullptr;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelSequenceActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		CurrentSequencer = Cast<ALevelSequenceActor>(FoundActors[0]);
	}

	return CurrentSequencer;
}
