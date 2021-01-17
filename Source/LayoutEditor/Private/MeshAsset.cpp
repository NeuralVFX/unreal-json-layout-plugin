// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#include "MeshAsset.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "MovieScene.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/ObjectLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h" 
#include "Tracks/MovieScene3DTransformTrack.h"


void UMeshAsset::Init(ALevelSequenceActor* CurrentSequencer, FString NewSceneName, FString NewContentName)
{
	SceneName = NewSceneName;
	ContentName = NewContentName;
	Sequencer = CurrentSequencer;
	World = Sequencer->GetWorld();
	Type = "AbstractMesh";

	CutName = UHelpers::MakePrettyContentName(ContentName);
}


AStaticMeshActor* UMeshAsset::MakeActor()
{
	//Spawn empty actor
	FActorSpawnParameters Params;
	AStaticMeshActor* SpawnedActor;
	SpawnedActor = World->SpawnActor<AStaticMeshActor>(Params);

	// Rename spawned actor
	USceneComponent* Component = SpawnedActor->GetRootComponent();
	SpawnedActor->SetActorLabel(SceneName);

	// Get asset from content browser
	FStringAssetReference asset_stream_ref(ContentName);
	TAssetPtr<UStaticMesh> mesh_asset(asset_stream_ref);
	UStaticMesh* mesh = mesh_asset.LoadSynchronous();
	UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Component);

	// Set asset
	MeshComponent->SetStaticMesh(mesh);

	return SpawnedActor;
}



