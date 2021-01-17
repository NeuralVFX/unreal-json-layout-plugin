// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#include "Helpers.h"
#include "Asset.h"
#include "AssetBox.h"
#include "Components/TextBlock.h"
#include "LayoutWidget.h"
#include "Engine/Selection.h"
#include "ISequencer.h"
#include "Kismet/GameplayStatics.h"
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
#include "Interfaces/IMainFrameModule.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Sections/MovieScene3DTransformSection.h"
#include "Tracks/MovieScene3DTransformTrack.h"
#include "LevelSequenceEditor/Private/LevelSequenceEditorToolkit.h"


UTextBlock* UHelpers::MakeTextBlock(FString Text, int Size, FLinearColor Color)
{
	// Create text object
	UTextBlock* ObjectTypeText = NewObject<UTextBlock>();
	ObjectTypeText->SetText(FText::FromString(Text));

	// Create font object
	FString Roboto = FPaths::ProjectPluginsDir() + "/LayoutEditor/Resources/Roboto-Medium.ttf";
	TSharedPtr<const FCompositeFont> MyInCompositeFont(new FCompositeFont(TEXT("MyRobotoFont"),
														Roboto,
														EFontHinting::Default,
														EFontLoadingPolicy::LazyLoad));
	// Set font
	FSlateFontInfo font(MyInCompositeFont, Size);
	ObjectTypeText->SetFont(font);

	// Set color
	ObjectTypeText->SetColorAndOpacity(FSlateColor(Color));

	return ObjectTypeText;
}


ULevelSequence* UHelpers::GetSequence(ALevelSequenceActor* CurrentSequencer)
{
	return Cast<ULevelSequence>(CurrentSequencer->LevelSequence.TryLoad());
}


void UHelpers::UpdateSequencer(ALevelSequenceActor* CurrentSequencer)
{
	// Try to open sequencer window
	FAssetEditorManager& AssetEditorManager = FAssetEditorManager::Get();
	ULevelSequence* Sequence = GetSequence(CurrentSequencer);
	AssetEditorManager.OpenEditorForAsset(Sequence);

	// Get sequencer window
	IAssetEditorInstance* AssetEditor = FAssetEditorManager::Get().FindEditorForAsset(Sequence, true);
	FLevelSequenceEditorToolkit* LevelSequenceEditor = (FLevelSequenceEditorToolkit*)(AssetEditor);

	if (LevelSequenceEditor != nullptr)
	{
		// Force sequencer update
		ISequencer* Sequencer = LevelSequenceEditor->GetSequencer().Get();
		Sequencer->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::RefreshAllImmediately);
	}
}


FFrameNumber UHelpers::GetFrameNumberTick(ALevelSequenceActor* CurrentSequencer, int Frame, bool Reverse)
{
	ULevelSequence* Sequence = GetSequence(CurrentSequencer);

	// Get both tick resolution and display rate
	FFrameRate TickResolution = Sequence->MovieScene->GetTickResolution();
	FFrameRate FrameRate = Sequence->MovieScene->GetDisplayRate();

	// Set time to query
	FFrameTime Time;
	Time.FrameNumber = Frame;

	FFrameNumber FrameTick;

	// Calculate conversion
	if (Reverse)
	{
		float Seconds = TickResolution.AsSeconds(Time);
		FrameTick = FrameRate.AsFrameNumber(Seconds);
	}
	else
	{
		float Seconds = FrameRate.AsSeconds(Time);
		FrameTick = TickResolution.AsFrameNumber(Seconds);
	}

	return FrameTick;
}


const void* UHelpers::GetParentWindow()
{
	const void* ParentWindowPtr = nullptr;
	IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	const TSharedPtr<SWindow>& MainFrameParentWindow = MainFrameModule.GetParentWindow();

	if (MainFrameParentWindow.IsValid() && MainFrameParentWindow->GetNativeWindow().IsValid())
	{
		ParentWindowPtr = MainFrameParentWindow->GetNativeWindow()->GetOSWindowHandle();
	}

	return ParentWindowPtr;
}


FGuid UHelpers::GetGuidFromSequencer(ALevelSequenceActor* CurrentSequencer, FString SceneName)
{
	// Get all bindings from sequncer
	ULevelSequence* Sequence = UHelpers::GetSequence(CurrentSequencer);
	UMovieScene* MovieScene = Sequence->GetMovieScene();
	const TArray < FMovieSceneBinding > Bindings = MovieScene->GetBindings();

	FGuid Guid;

	// Loop through bindings and find matching name
	for (FMovieSceneBinding Binding : Bindings)
	{
		FGuid BindingGUID = Binding.GetObjectGuid();
		FMovieScenePossessable* Possesable = MovieScene->FindPossessable(BindingGUID);
		if (Possesable && Possesable->GetName() == SceneName)
		{
			Guid = BindingGUID;
		}
	}

	return Guid;
}


AActor* UHelpers::GetActorFromSequencer(ALevelSequenceActor* CurrentSequencer, FGuid Guid)
{

	// Query object bound with Guid from sequencer
	ULevelSequence* Sequence = UHelpers::GetSequence(CurrentSequencer);
	TArray < UObject*, TInlineAllocator < 1 > > BoundObjs;
	Sequence->LocateBoundObjects(Guid, CurrentSequencer->GetWorld(), BoundObjs);

	AActor* Actor = nullptr;

	// Extract actor
	if (BoundObjs.Num() > 0)
	{
		TWeakObjectPtr<UObject> firstBoundObjPtr = BoundObjs[0];
		UObject *firstBoundObj = firstBoundObjPtr.Get();
		Actor = Cast<AActor>(firstBoundObj);
	}

	return Actor;
}


UMovieScene3DTransformSection* UHelpers::GetTransformSection(ALevelSequenceActor* CurrentSequencer, FGuid Guid)
{
	// Get tracks from sequencer
	ULevelSequence* Sequence = GetSequence(CurrentSequencer);
	FMovieSceneBinding* Binding = Sequence->MovieScene->FindBinding(Guid);
	TArray<UMovieSceneTrack*> Tracks = Binding->GetTracks();

	UMovieScene3DTransformSection* CurrentTranSection = nullptr;

	// Loop through tracks and sections to find 3d transform section
	for (UMovieSceneTrack* Track : Tracks)
	{
		UMovieScene3DTransformTrack* TranTrack = Cast<UMovieScene3DTransformTrack>(Track);
		if (TranTrack)
		{
			TArray<UMovieSceneSection*> Sections;
			Sections = TranTrack->GetAllSections();

			// Loop through sections
			for (UMovieSceneSection* Section : Sections)
			{
				// Check if transform found and set
				UMovieScene3DTransformSection* TranSection = Cast<UMovieScene3DTransformSection>(Section);
				if (Section)
				{
					CurrentTranSection = TranSection;
				}

			}
		}
	}

	return CurrentTranSection;
}


TArray<FMovieSceneFloatValue> UHelpers::MakeKeyArray(TArray<float> ValueArray)
{
	TArray<FMovieSceneFloatValue> KeyArray;

	// Loop through all values and build new MovieSceneFloatValue array
	for (float Value: ValueArray)
	{
		FMovieSceneFloatValue Key;
		Key.Value = Value;
		KeyArray.Add(Key);
	}

	return KeyArray;
}


FString  UHelpers::MakePrettyContentName(FString ContentPath)
{
	TArray<FString> Array;
	ContentPath.ParseIntoArray(Array, TEXT("/"), true);
	FString CuttingName = Array.Pop();
	TArray<FString> ArrayB;

	CuttingName.ParseIntoArray(ArrayB, TEXT("."), true);

	return ArrayB[0];
}