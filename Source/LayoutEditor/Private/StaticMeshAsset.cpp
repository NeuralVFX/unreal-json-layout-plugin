// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#include "StaticMeshAsset.h"



void UStaticMeshAsset::Init(ALevelSequenceActor* CurrentSequencer, FString NewSceneName, FString NewContentName, FTransform NewTransform)
{
	SceneName = NewSceneName;
	ContentName = NewContentName;
	Transform = NewTransform;
	Sequencer = CurrentSequencer;
	World = Sequencer->GetWorld();
	Type = "StaticMesh";

	CutName = UHelpers::MakePrettyContentName(ContentName);
}


void UStaticMeshAsset::Load()
{

	if (!IsLoaded())
	{

		// Spawn Actor
		AStaticMeshActor*SpawnedActor = MakeActor();

		// Add to Sequencer
		ULevelSequence * Sequence = UHelpers::GetSequence(Sequencer);
		FGuid Guid;
		Guid = Sequence->MovieScene->AddPossessable(SpawnedActor->GetActorLabel(), SpawnedActor->GetClass());
		Sequence->BindPossessableObject(Guid, *SpawnedActor, SpawnedActor->GetWorld());

		// Set actor transform
		SpawnedActor->SetActorTransform(Transform);
	}
}
