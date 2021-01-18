// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#include "AnimatedMeshAsset.h"
#include "JsonLayout.h"
#include "Helpers.h"
#include "MovieSceneTrack.h"
#include "Sections/MovieScene3DTransformSection.h"
#include "Channels/MovieSceneFloatChannel.h"
#include "Channels/MovieSceneChannelProxy.h"
#include "MovieScene.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/ObjectLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h" 
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Tracks/MovieScene3DTransformTrack.h"


void UAnimatedMeshAsset::Init(ALevelSequenceActor* CurrentSequencer, FString NewSceneName, FString NewContentName, FAnimatedStaticMeshObject NewJsonData)
{
	SceneName = NewSceneName;
	ContentName = NewContentName;
	JsonData = NewJsonData;
	Sequencer = CurrentSequencer;
	World = Sequencer->GetWorld();
	Type = "Animated StaticMesh";

	CutName = UHelpers::MakePrettyContentName(ContentName);
}


void UAnimatedMeshAsset::Load()
{
	if (!IsLoaded())
	{

		// Spawn actor
		AStaticMeshActor* SpawnedActor = MakeActor();

		// Add to Sequencer
		ULevelSequence* Sequence = UHelpers::GetSequence(Sequencer);

		FGuid Guid;
		Guid = Sequence->MovieScene->AddPossessable(SpawnedActor->GetActorLabel(), SpawnedActor->GetClass());
		Sequence->BindPossessableObject(Guid, *SpawnedActor, SpawnedActor->GetWorld());

		// Add track and section to sequencer
		UMovieScene3DTransformTrack* NewTrack = Cast<UMovieScene3DTransformTrack>(Sequence->MovieScene->AddTrack(UMovieScene3DTransformTrack::StaticClass(), Guid));
		UMovieSceneSection* Section = NewTrack->CreateNewSection();

		// Check that keyframes exist
		if (JsonData.frame.Num() > 0)
		{
		
			// Set time range
			int EndFrame = JsonData.frame.Last();
			int StartFrame = JsonData.frame[0];

			Section->SetStartFrame(UHelpers::GetFrameNumberTick(Sequencer, StartFrame, false));
			Section->SetEndFrame(UHelpers::GetFrameNumberTick(Sequencer, EndFrame, false));

			NewTrack->AddSection(*Cast<UMovieScene3DTransformSection>(Section));

			// Fetch keyframe object from section
			const FMovieSceneChannelProxy& channelProxy = Section->GetChannelProxy();
			auto ChannelArray = channelProxy.GetChannels<FMovieSceneFloatChannel>();

			// Set frame numbers
			TArray<FFrameNumber> FrameArray;
			for (int frame : JsonData.frame)
			{
				FFrameNumber Frame = UHelpers::GetFrameNumberTick(Sequencer, frame, false);
				FrameArray.Add(Frame);
			}

			// Set keyframes
			ChannelArray[0]->Set(FrameArray, UHelpers::MakeKeyArray(JsonData.tx));
			ChannelArray[1]->Set(FrameArray, UHelpers::MakeKeyArray(JsonData.ty));
			ChannelArray[2]->Set(FrameArray, UHelpers::MakeKeyArray(JsonData.tz));
			ChannelArray[3]->Set(FrameArray, UHelpers::MakeKeyArray(JsonData.rx));
			ChannelArray[4]->Set(FrameArray, UHelpers::MakeKeyArray(JsonData.ry));
			ChannelArray[5]->Set(FrameArray, UHelpers::MakeKeyArray(JsonData.rz));
			ChannelArray[6]->Set(FrameArray, UHelpers::MakeKeyArray(JsonData.sx));
			ChannelArray[7]->Set(FrameArray, UHelpers::MakeKeyArray(JsonData.sy));
			ChannelArray[8]->Set(FrameArray, UHelpers::MakeKeyArray(JsonData.sz));
		}
	}
}
