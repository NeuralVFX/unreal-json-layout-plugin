// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeshAsset.h"
#include "JsonLayout.h"
#include "AnimatedMeshAsset.generated.h"

/**
 * 
 */
UCLASS()
class LAYOUTEDITOR_API UAnimatedMeshAsset : public UMeshAsset
{
	GENERATED_BODY()

		FAnimatedStaticMeshObject JsonData;

public:

	virtual void Init(ALevelSequenceActor *CurrentSequencer, FString NewSceneName, FString NewContentName, FAnimatedStaticMeshObject NewJsonData);

	virtual bool Load() override;
	
};
