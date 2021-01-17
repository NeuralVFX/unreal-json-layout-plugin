// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeshAsset.h"
#include "StaticMeshAsset.generated.h"

/** Child of MeshAsset Class - Represents Non Moving Static Mesh Loaded From Json */
UCLASS()
class LAYOUTEDITOR_API UStaticMeshAsset : public UMeshAsset
{
	GENERATED_BODY()

public:

	/** Transform of object in scene */
	struct FTransform Transform;

public:

	/**
	* Initiate the variables of the asset.
	* @param CurrentSequencer - Sequence Actor which will load this asset.
	* @param NewSceneName - Scene name of the asset.
	* @param NewContentName - Content Library path of the asset.
	* @param NewTransform - Transform Matrix for the assets location in the scene.
	*/
	virtual void Init(ALevelSequenceActor *CurrentSequencer, FString NewSceneName, FString NewContentName, FTransform NewTransform);

	/**
	* Create the Actor associated with this asset, add to the sequencer, and set transform.
	*/
	virtual void Load() override;

};
