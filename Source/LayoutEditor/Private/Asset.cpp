// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#include "Asset.h"
#include "Helpers.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "MovieScene.h"
#include "Engine/StaticMesh.h"
#include "Engine/ObjectLibrary.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h" 
#include "Tracks/MovieScene3DTransformTrack.h"


UAsset::UAsset()
{
}


UAsset::~UAsset()
{
}


void UAsset::Init(ALevelSequenceActor* CurrentSequencer, FString NewSceneName)
{
	SceneName = NewSceneName;
	Sequencer = CurrentSequencer;

	World = Sequencer->GetWorld();
	Type = "Abstract";
}



FGuid UAsset::GetGuid()
{
	FGuid Guid = UHelpers::GetGuidFromSequencer(Sequencer, SceneName);
	return Guid;
}


AActor* UAsset::GetActor()
{
	FGuid Guid = GetGuid();
	AActor* Actor = UHelpers::GetActorFromSequencer(Sequencer,Guid);

	return Actor;
}


void UAsset::Load()
{

}


void UAsset::Unload()
{
	if (IsLoaded())
	{
		// Get required arguments
		FGuid Guid = GetGuid();
		AActor* Actor = GetActor();

		// Remove object from sequencer
		ULevelSequence* Sequence = UHelpers::GetSequence(Sequencer);
		Sequence->GetMovieScene()->RemovePossessable(Guid);
		Sequence->UnbindPossessableObjects(Guid);

		// Remove actor from scene
		if (Actor)
		{
			Actor->Destroy();
		}

		// Make sure Guid is no longer valid
		Guid.Invalidate();
	}
}


bool UAsset::IsLoaded()
{
	// Check sequencer to see if object is loaded in scene
	FGuid Guid = GetGuid();

	return Guid.IsValid();
}