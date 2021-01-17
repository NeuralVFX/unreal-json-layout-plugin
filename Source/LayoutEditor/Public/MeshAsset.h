// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset.h"
#include "MeshAsset.generated.h"

/** Child of Asset Class, representing Mesh Assets Loaded From Json */
UCLASS()
class LAYOUTEDITOR_API UMeshAsset : public UAsset
{
	GENERATED_BODY()

public:

	/**
	* Create Actor in the scene.
	* @return Created Actor.
	*/
	virtual class AStaticMeshActor*  MakeActor();

	/**
	* Initiate the variables of the asset.
	* @param CurrentSequencer - Sequence Actor which will load this asset.
	* @param NewSceneName - Scene name of the asset.
	* @param NewContentName - Content Library path of the asset.
	*/
	virtual void Init(ALevelSequenceActor *CurrentSequencer, FString NewSceneName, FString NewContentName);

};

