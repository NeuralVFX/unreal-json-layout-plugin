// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimatedMeshAsset.h"
#include "JsonLayout.h"
#include "Helpers.h"
#include "MovieSceneTrack.h"
#include "Sections/MovieScene3DTransformSection.h"
#include "Channels/MovieSceneFloatChannel.h"
#include "Channels/MovieSceneChannelProxy.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "MovieScene.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h" 
#include "Engine/StaticMesh.h"
#include "Engine/ObjectLibrary.h"
#include "Tracks/MovieScene3DTransformTrack.h"



void UAnimatedMeshAsset::Init(ALevelSequenceActor* CurrentSequencer, FString NewSceneName, FString NewContentName, FAnimatedStaticMeshObject NewJsonData)
{
	SceneName = NewSceneName;
	ContentName = NewContentName;
	JsonData = NewJsonData;
	Sequencer = CurrentSequencer;
	World = Sequencer->GetWorld();
	Type = "Animated StaticMesh";

	TArray<FString> Array;
	ContentName.ParseIntoArray(Array, TEXT("/"), true);
	FString CuttingName = Array.Pop();
	TArray<FString> ArrayB;

	CuttingName.ParseIntoArray(ArrayB, TEXT("."), true);
	CutName = ArrayB[0];
}




bool UAnimatedMeshAsset::Load()
{

	if (!IsLoaded())
	{

		// Spawn Actor
		AStaticMeshActor*SpawnedActor = MakeActor();

		// Add to Sequencer
		ULevelSequence * Sequence = UHelpers::GetSequence(Sequencer);

		UE_LOG(LogTemp, Warning, TEXT("Name %s"), *Sequencer->LevelSequence.GetAssetName());
		FGuid Guid;
		Guid = Sequence->MovieScene->AddPossessable(SpawnedActor->GetActorLabel(), SpawnedActor->GetClass());
		Sequence->BindPossessableObject(Guid, *SpawnedActor, SpawnedActor->GetWorld());
		UMovieScene3DTransformTrack* NewTrack = Cast<UMovieScene3DTransformTrack>(Sequence->MovieScene->AddTrack(UMovieScene3DTransformTrack::StaticClass(), Guid));
		UMovieSceneSection * Section = NewTrack->CreateNewSection();

		int EndFrame = JsonData.frame.Last();
		int StartFrame = JsonData.frame[0];

		Section->SetStartFrame(UHelpers::GetFrameNumberTick(Sequencer, StartFrame, false));
		Section->SetEndFrame(UHelpers::GetFrameNumberTick(Sequencer, EndFrame, false));

		NewTrack->AddSection(*Cast<UMovieScene3DTransformSection>(Section));


		const FMovieSceneChannelProxy& channelProxy = Section->GetChannelProxy();
		auto ChannelArray = channelProxy.GetChannels<FMovieSceneFloatChannel>();



		TArray<FFrameNumber> FrameArray;
		for (int frame : JsonData.frame)
		{
			FFrameNumber Frame = UHelpers::GetFrameNumberTick(Sequencer, frame, false);
			FrameArray.Add(Frame);
		}
		float zhe = UHelpers::MakeKeyArray(JsonData.tx).Num();
		float na = FrameArray.Num();
		UE_LOG(LogTemp, Warning, TEXT("KeyCount %f %f"), zhe,na);

		
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
	return true;
}
