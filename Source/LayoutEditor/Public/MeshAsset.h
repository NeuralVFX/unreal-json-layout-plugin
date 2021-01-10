// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Asset.h"
#include "MeshAsset.generated.h"

/**
 * 
 */
UCLASS()
class LAYOUTEDITOR_API UMeshAsset : public UAsset
{
	GENERATED_BODY()


public:

	virtual class AStaticMeshActor*  MakeActor();

	virtual void Init(ALevelSequenceActor *CurrentSequencer, FString NewSceneName, FString NewContentName);

};
