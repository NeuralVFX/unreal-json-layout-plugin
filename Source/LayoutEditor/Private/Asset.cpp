// Fill out your copyright notice in the Description page of Project Settings.


#include "Asset.h"
#include "Helpers.h"

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
	AActor *Actor = UHelpers::GetActorFromSequencer(Sequencer,Guid);
	return Actor;

}


bool UAsset::Load()
{
	return true;
}


bool UAsset::Unload()
{
	if (IsLoaded())
	{
		FGuid Guid = GetGuid();
		AActor* Actor = GetActor();
		ULevelSequence * Sequence = UHelpers::GetSequence(Sequencer);
		Sequence->GetMovieScene()->RemovePossessable(Guid);
		Sequence->UnbindPossessableObjects(Guid);

		Actor->Destroy();
		Guid.Invalidate();
	}
	return true;
}

bool UAsset::IsLoaded()
{
	FGuid Guid = GetGuid();
	return Guid.IsValid();
}