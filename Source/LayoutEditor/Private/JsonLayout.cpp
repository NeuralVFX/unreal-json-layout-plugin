// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#include "JsonLayout.h"
#include "Kismet/KismetSystemLibrary.h"
#include "JsonUtilities.h"
#include "Helpers.h"
#include "Serialization/JsonReader.h"
#include "UObject/UObjectBaseUtility.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Sections/MovieScene3DTransformSection.h"
#include "Tracks/MovieScene3DTransformTrack.h"
#include "Channels/MovieSceneFloatChannel.h"
#include "Channels/MovieSceneChannelProxy.h"


bool UJsonLayout::WriteLayoutData(FString LayoutName, TArray<AStaticMeshActor*> ActorArray, ALevelSequenceActor* Sequencer)
{
	JsonObjectPtr JsonRootObject = MakeShareable(new FJsonObject);
	FStaticMeshArray MeshArray;

	// Loop through actors and create structs for JSON
	for (AStaticMeshActor* Actor : ActorArray)
	{
		FGuid Guid = UHelpers::GetGuidFromSequencer(Sequencer, UKismetSystemLibrary::GetDisplayName(Actor));

		// Check if actor has animation and record data accordingly
		if (Guid.IsValid())
		{
			FAnimatedStaticMeshObject AnimatedMesh = MakeAnimatedStaticMesh(Sequencer, Actor, Guid);
			MeshArray.AnimatedStaticMeshes.Add(AnimatedMesh);

		}
		else
		{
			FStaticMeshObject Mesh = MakeStaticMesh(Actor);
			MeshArray.StaticMeshes.Add(Mesh);
		}
	}

	// Convert struct to string
	FString JSONPayload;
	FJsonObjectConverter::UStructToJsonObjectString(MeshArray, JSONPayload, 0, 0);

	// Save to JSON file
	return FFileHelper::SaveStringToFile(JSONPayload, *LayoutName);
}


FStaticMeshArray UJsonLayout::ReadLayoutData(FString LayoutName)
{
	// Open JSON as string
	FString LayoutString;
	FFileHelper::LoadFileToString(LayoutString, *LayoutName);

	// Convert string to struct
	FStaticMeshArray StaticMeshes;
	FJsonObjectConverter::JsonObjectStringToUStruct(LayoutString, &StaticMeshes,0,0);

	return StaticMeshes;
}


FStaticMeshObject  UJsonLayout::MakeStaticMesh(AStaticMeshActor* Actor)
{
	// Extract scene name and content name
	FStaticMeshObject Mesh;
	Mesh.ContentName = Cast<AStaticMeshActor>(Actor)->GetStaticMeshComponent()->GetStaticMesh()->GetPathName();
	Mesh.SceneName = UKismetSystemLibrary::GetDisplayName(Actor);

	// Extract transformation matrix
	FMatrix Matrix = Actor->GetTransform().ToMatrixWithScale();
	Mesh.Matrix = Matrix;

	return Mesh;
}


FAnimatedStaticMeshObject  UJsonLayout::MakeAnimatedStaticMesh(ALevelSequenceActor* Sequencer, AStaticMeshActor* Actor, FGuid Guid)
{
	// Extract scene name and content name
	FAnimatedStaticMeshObject Mesh;
	Mesh.ContentName = Cast<AStaticMeshActor>(Actor)->GetStaticMeshComponent()->GetStaticMesh()->GetPathName();
	Mesh.SceneName = UKismetSystemLibrary::GetDisplayName(Actor);


	// Get transformation track from sequencer
	UMovieScene3DTransformSection* CurrentTranSection = UHelpers::GetTransformSection( Sequencer, Guid);

	// Record values from sequencer into struct
	if (CurrentTranSection)
	{ 
		// Get start and end frame
		FFrameNumber StartFrame = UHelpers::GetFrameNumberTick(Sequencer, CurrentTranSection->GetInclusiveStartFrame().Value, true);
		FFrameNumber EndFrame = UHelpers::GetFrameNumberTick(Sequencer, CurrentTranSection->GetExclusiveEndFrame().Value, true);

		// Get transform channels
		const FMovieSceneChannelProxy& channelProxy = CurrentTranSection->GetChannelProxy();
		auto ChannelArray = channelProxy.GetChannels<FMovieSceneFloatChannel>();

		TArray<FMovieSceneFloatValue> KeyArray;
		TArray<FFrameNumber> FrameArray;

		// Loop through frames and set key values in struct
		for (int i = StartFrame.Value; i < EndFrame.Value; i++)
		{
			FFrameNumber Frame = UHelpers::GetFrameNumberTick(Sequencer, i, false);

			float tx;
			ChannelArray[0]->Evaluate(Frame, tx);
			Mesh.tx.Add( tx);

			float ty;
			ChannelArray[1]->Evaluate(Frame, ty);
			Mesh.ty.Add( ty);

			float tz;
			ChannelArray[2]->Evaluate(Frame, tz);
			Mesh.tz.Add( tz);

			float rx;
			ChannelArray[3]->Evaluate(Frame, rx);
			Mesh.rx.Add( rx);

			float ry;
			ChannelArray[4]->Evaluate(Frame, ry);
			Mesh.ry.Add( ry);

			float rz;
			ChannelArray[5]->Evaluate(Frame, rz);
			Mesh.rz.Add( rz);

			float sx;
			ChannelArray[6]->Evaluate(Frame, sx);
			Mesh.sx.Add( sx);

			float sy;
			ChannelArray[7]->Evaluate(Frame, sy);
			Mesh.sy.Add( sy);

			float sz;
			ChannelArray[8]->Evaluate(Frame, sz);
			Mesh.sz.Add( sz);

			Mesh.frame.Add(  i);
		}
	}

	return Mesh;
}

