// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticMeshAsset.h"




void UStaticMeshAsset::Init(ALevelSequenceActor* CurrentSequencer, FString NewSceneName, FString NewContentName, FTransform NewTransform)
{
	SceneName = NewSceneName;
	ContentName = NewContentName;
	Transform = NewTransform;
	Sequencer = CurrentSequencer;
	World = Sequencer->GetWorld();
	Type = "StaticMesh";

	TArray<FString> Array;
	ContentName.ParseIntoArray(Array, TEXT("/"), true);
	FString CuttingName = Array.Pop();
	TArray<FString> ArrayB;

	CuttingName.ParseIntoArray(ArrayB, TEXT("."), true);
	CutName = ArrayB[0];
}




bool UStaticMeshAsset::Load()
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

		/*
		UMovieScene3DTransformTrack* NewTrack = Cast<UMovieScene3DTransformTrack>(Sequence->MovieScene->AddTrack(UMovieScene3DTransformTrack::StaticClass(), Guid));
		UMovieSceneSection * Section = NewTrack->CreateNewSection();

		Section->SetStartFrame(UHelpers::GetFrameNumberTick(Sequencer, 0, false));
		Section->SetEndFrame(UHelpers::GetFrameNumberTick(Sequencer, 50, false));

		NewTrack->AddSection(*Cast<UMovieScene3DTransformSection>(Section));


		const FMovieSceneChannelProxy& channelProxy = Section->GetChannelProxy();
		auto ChannelArray = channelProxy.GetChannels<FMovieSceneFloatChannel>();


		TArray<FMovieSceneFloatValue> KeyArray;
		TArray<FFrameNumber> FrameArray;
		for (int i = 0; i < 50; i++)
		{
			FFrameNumber Frame = UHelpers::GetFrameNumberTick(Sequencer, i, false);
			FMovieSceneFloatValue Key;
			Key.Value = i * 5;
			KeyArray.Add(Key);
			FrameArray.Add(Frame);

		}


		ChannelArray[0]->Set(FrameArray, KeyArray);
				*/
		SpawnedActor->SetActorTransform(Transform);
	}
	return true;
}
