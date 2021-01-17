// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeshAsset.h"
#include "JsonLayout.h"
#include "AnimatedMeshAsset.generated.h"

/** Child of MeshAsset Class - Represents Non Moving Static Mesh Loaded From Json */
UCLASS()
class LAYOUTEDITOR_API UAnimatedMeshAsset : public UMeshAsset
{
	GENERATED_BODY()

public:

	/** Animated transforms stored in a struct */
	FAnimatedStaticMeshObject JsonData;

public:

	/**
	* Initiate the variables of the asset.
	* @param CurrentSequencer - Sequence Actor which will load this asset.
	* @param NewSceneName - Scene name of the asset.
	* @param NewContentName - Content Library path of the asset.
	* @param NewJsonData - Struct containing animated keyframes for sequencer.
	*/
	virtual void Init(ALevelSequenceActor *CurrentSequencer, FString NewSceneName, FString NewContentName, FAnimatedStaticMeshObject NewJsonData);

	/**
	* Create the Actor associated with this asset, add to the sequencer, and add keys to sequencer.
	*/
	virtual void Load() override;
	
};
