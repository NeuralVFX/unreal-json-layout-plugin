// Fill out your copyright notice in the Description page of Project Settings.

#include "Helpers.h"
#include "Components/TextBlock.h"
#include "LayoutWidget.h"
#include "Engine/Selection.h"
#include "Asset.h"
#include "AssetBox.h"
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
#include "C:/Program Files/Epic Games/UE_4.26/Engine/Plugins/MovieScene/LevelSequenceEditor/Source/LevelSequenceEditor/Private/LevelSequenceEditorToolkit.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "ISequencer.h"
#include "Interfaces/IMainFrameModule.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Sections/MovieScene3DTransformSection.h"
#include "Tracks/MovieScene3DTransformTrack.h"


UTextBlock * UHelpers::MakeTextBlock(FString Text, int Size, FLinearColor Color)
{

	UTextBlock *ObjectTypeText = NewObject<UTextBlock>();
	ObjectTypeText->SetText(FText::FromString(Text));
	TSharedPtr<const FCompositeFont> MyInCompositeFont(new FCompositeFont(TEXT("MyRobotoFont"),
														"C:/Users/charl/Documents/Unreal Projects/EditorUI/Content/roboto/Roboto-Medium.ttf",
														EFontHinting::Default,
														EFontLoadingPolicy::LazyLoad));
	FSlateFontInfo font(MyInCompositeFont, Size);
	ObjectTypeText->SetFont(font);
	ObjectTypeText->SetColorAndOpacity(FSlateColor(Color));
	return ObjectTypeText;

}

ULevelSequence * UHelpers::GetSequence(ALevelSequenceActor* CurrentSequencer)
{
	return Cast<ULevelSequence>(CurrentSequencer->LevelSequence.TryLoad());
}


void UHelpers::UpdateSequencer(ALevelSequenceActor* CurrentSequencer)
{
	FAssetEditorManager& AssetEditorManager = FAssetEditorManager::Get();
	ULevelSequence *Sequence = GetSequence(CurrentSequencer);
	AssetEditorManager.OpenEditorForAsset(Sequence);
	IAssetEditorInstance* AssetEditor = FAssetEditorManager::Get().FindEditorForAsset(Sequence, true);
	FLevelSequenceEditorToolkit* LevelSequenceEditor = (FLevelSequenceEditorToolkit*)(AssetEditor);

	if (LevelSequenceEditor != nullptr)

	{
		ISequencer* Sequencer = LevelSequenceEditor->GetSequencer().Get();
		Sequencer->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::RefreshAllImmediately);

	}
}


FFrameNumber UHelpers::GetFrameNumberTick(ALevelSequenceActor* CurrentSequencer, int Frame, bool Reverse)
{
	ULevelSequence *Sequence = GetSequence(CurrentSequencer);
	FFrameRate TickResolution = Sequence->MovieScene->GetTickResolution();
	FFrameRate FrameRate = Sequence->MovieScene->GetDisplayRate();
	FFrameTime Time;
	Time.FrameNumber = Frame;

	FFrameNumber FrameTick;
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
	FGuid Guid;

	ULevelSequence * Sequence = UHelpers::GetSequence(CurrentSequencer);
	UMovieScene *MovieScene = Sequence->GetMovieScene();
	const TArray < FMovieSceneBinding > Bindings = MovieScene->GetBindings();

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
	AActor *Actor = nullptr;

	ULevelSequence * Sequence = UHelpers::GetSequence(CurrentSequencer);
	TArray < UObject *, TInlineAllocator < 1 > > boundObjs;

	/// connection between binding id and actor reference
	Sequence->LocateBoundObjects(Guid, CurrentSequencer->GetWorld(), boundObjs);

	if (boundObjs.Num() > 0)
	{
		TWeakObjectPtr<UObject> firstBoundObjPtr = boundObjs[0];
		UObject *firstBoundObj = firstBoundObjPtr.Get();
		Actor = Cast<AActor>(firstBoundObj);
	}

	return Actor;
}


UMovieScene3DTransformSection* UHelpers::GetTransformSection(ALevelSequenceActor* CurrentSequencer, FGuid Guid)
{
	ULevelSequence* Sequence = GetSequence(CurrentSequencer);
	FMovieSceneBinding * Binding = Sequence->MovieScene->FindBinding(Guid);
	TArray<UMovieSceneTrack*> Tracks = Binding->GetTracks();
	UMovieScene3DTransformSection* CurrentTranSection = nullptr;

	for (UMovieSceneTrack * Track : Tracks)
	{
		UMovieScene3DTransformTrack* TranTrack = Cast<UMovieScene3DTransformTrack>(Track);
		if (TranTrack)
		{
			TArray<UMovieSceneSection*> Sections;
			Sections = TranTrack->GetAllSections();

			for (UMovieSceneSection* Section : Sections)
			{
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
	for (float Value: ValueArray)
	{
		FMovieSceneFloatValue Key;
		Key.Value = Value;
		KeyArray.Add(Key);
	}
	return KeyArray;
}