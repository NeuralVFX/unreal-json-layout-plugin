// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Asset.generated.h"

/** 
* Abstract Class Representing Assets Loaded From Json 
*
* Mangages loading and unloading from scene
* Uses sequencer to keep track of state
*
* Functions "Load" "Unload" and "IsLoaded"; can be overridden to make any Class type
*/
UCLASS(abstract)
class LAYOUTEDITOR_API UAsset : public UObject
{
	GENERATED_BODY()
	

public:
	UAsset();
	~UAsset();

	/** Sequence actor to load asset into */
	UPROPERTY(EditAnywhere, Category = "Sequencer")
	class ALevelSequenceActor * Sequencer;

	/** World object of scene */
	class UWorld * World;

	/** Name asset will be given in scenefile */
	FString SceneName;

	/** Content library path for asset */
	FString ContentName;

	/** Display friendly name for content library path */
	FString CutName;

	/** Asset type ie:(StaticMesh, Animated StaticMesh, Camera) */
	FString Type;

public:

	/**
	 * Initiate the variables of the asset.
	 * @param CurrentSequencer - Sequence Actor which will load this asset.
	 * @param NewSceneName - Scene name of the asset.
	 */
	virtual void Init(ALevelSequenceActor *CurrentSequencer, FString NewSceneName);

	/**
	 * Lookup the Actor in the scene associated with this asset.
	 * @return The Actor associated with this asset.
	 */
	class AActor * GetActor();

	/**
	 * Lookup the FGuid in the LevelSequence associated with this asset.
	 * @return The FGuid associated with this asset.
	 */
	FGuid GetGuid();

	/**
	 * Create the Actor associated with this asset, and add to the sequencer.
	 */
	virtual void Load();

	/**
	 * Delete the Actor associated with this asset, and remove from the sequencer.
	 */
	virtual void Unload();

	/**
	 * Check sequencer to see if the asset is currently loaded in the scene.
	 * @return Whether this asset is loaded in the scene and sequencer.
	 */
	virtual bool IsLoaded();

};
