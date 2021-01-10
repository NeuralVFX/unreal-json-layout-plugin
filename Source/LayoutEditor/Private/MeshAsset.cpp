// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshAsset.h"

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


void UMeshAsset::Init(ALevelSequenceActor* CurrentSequencer, FString NewSceneName, FString NewContentName)
{
	SceneName = NewSceneName;
	ContentName = NewContentName;
	Sequencer = CurrentSequencer;
	World = Sequencer->GetWorld();
	Type = "AbstractMesh";

	TArray<FString> Array;
	ContentName.ParseIntoArray(Array, TEXT("/"), true);
	FString CuttingName = Array.Pop();
	TArray<FString> ArrayB;

	CuttingName.ParseIntoArray(ArrayB, TEXT("."), true);
	CutName = ArrayB[0];
}


AStaticMeshActor* UMeshAsset::MakeActor()
{
	//Spawn Empty Actor
	FActorSpawnParameters Params;
	AStaticMeshActor* SpawnedActor;
	SpawnedActor = World->SpawnActor<AStaticMeshActor>(Params);

	USceneComponent* Component = SpawnedActor->GetRootComponent();
	SpawnedActor->SetActorLabel(SceneName);


	// Get Asset From Content Browser
	FStringAssetReference asset_stream_ref(ContentName);
	TAssetPtr<UStaticMesh> mesh_asset(asset_stream_ref);
	UStaticMesh* mesh = mesh_asset.LoadSynchronous();
	UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Component);

	// Set Asset
	MeshComponent->SetStaticMesh(mesh);
	return SpawnedActor;
}



