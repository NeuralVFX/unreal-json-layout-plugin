// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeshAsset.h"
#include "StaticMeshAsset.generated.h"

/**
 * 
 */
UCLASS()
class LAYOUTEDITOR_API UStaticMeshAsset : public UMeshAsset
{
	GENERATED_BODY()

public:

	struct FTransform Transform;

public:

	virtual void Init(ALevelSequenceActor *CurrentSequencer, FString NewSceneName, FString NewContentName, FTransform NewTransform);

	virtual bool Load() override;

};
