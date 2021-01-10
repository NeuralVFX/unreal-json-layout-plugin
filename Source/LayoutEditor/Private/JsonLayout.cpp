// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetSystemLibrary.h"
#include "JsonLayout.h"
#include "JsonUtilities.h"
#include "Helpers.h"
#include "Serialization/JsonReader.h"
#include "UObject/UObjectBaseUtility.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Sections/MovieScene3DTransformSection.h"
#include "Tracks/MovieScene3DTransformTrack.h"
#include "Channels/MovieSceneFloatChannel.h"
#include "Channels/MovieSceneChannelProxy.h"



static const FString WritePath(FPaths::ProjectSavedDir() / "LayoutJson");


bool UJsonLayout::WriteLayoutData(FString LayoutName, TArray<AStaticMeshActor*> ActorArray, ALevelSequenceActor * Sequencer)
{
	JsonObjectPtr JsonRootObject = MakeShareable(new FJsonObject);
	FStaticMeshArray MeshArray;

	for (AStaticMeshActor* Actor : ActorArray)
	{
		FGuid Guid = UHelpers::GetGuidFromSequencer(Sequencer, UKismetSystemLibrary::GetDisplayName(Actor));
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

	FString JSONPayload;
	FJsonObjectConverter::UStructToJsonObjectString(MeshArray, JSONPayload, 0, 0);
	return FFileHelper::SaveStringToFile(JSONPayload, * LayoutName);
}


FStaticMeshArray UJsonLayout::ReadLayoutData(FString LayoutName)
{
	FString LayoutString;
	FFileHelper::LoadFileToString(LayoutString, *LayoutName);

	FStaticMeshArray StaticMeshes;
	FJsonObjectConverter::JsonObjectStringToUStruct(LayoutString, &StaticMeshes,0,0);
	return StaticMeshes;

}

FStaticMeshObject  UJsonLayout::MakeStaticMesh(AStaticMeshActor* Actor)
{
	FStaticMeshObject Mesh;
	Mesh.ContentName = Cast<AStaticMeshActor>(Actor)->GetStaticMeshComponent()->GetStaticMesh()->GetPathName();
	Mesh.SceneName = UKismetSystemLibrary::GetDisplayName(Actor);
	FMatrix Matrix = Actor->GetTransform().ToMatrixWithScale();
	Mesh.Matrix = Matrix;
	return Mesh;
}

FAnimatedStaticMeshObject  UJsonLayout::MakeAnimatedStaticMesh(ALevelSequenceActor * Sequencer, AStaticMeshActor* Actor, FGuid Guid)
{
	FAnimatedStaticMeshObject Mesh;
	Mesh.ContentName = Cast<AStaticMeshActor>(Actor)->GetStaticMeshComponent()->GetStaticMesh()->GetPathName();
	Mesh.SceneName = UKismetSystemLibrary::GetDisplayName(Actor);



	UMovieScene3DTransformSection* CurrentTranSection = UHelpers::GetTransformSection( Sequencer, Guid);


	if (CurrentTranSection)
	{ 
		FFrameNumber StartFrame = UHelpers::GetFrameNumberTick(Sequencer, CurrentTranSection->GetInclusiveStartFrame().Value, true);
		FFrameNumber EndFrame = UHelpers::GetFrameNumberTick(Sequencer, CurrentTranSection->GetExclusiveEndFrame().Value, true);

		const FMovieSceneChannelProxy& channelProxy = CurrentTranSection->GetChannelProxy();
		auto ChannelArray = channelProxy.GetChannels<FMovieSceneFloatChannel>();

		TArray<FMovieSceneFloatValue> KeyArray;
		TArray<FFrameNumber> FrameArray;
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

