// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Asset.generated.h"

/**
 * 
 */
UCLASS(abstract)
class LAYOUTEDITOR_API UAsset : public UObject
{
	GENERATED_BODY()
	

public:
	UAsset();
	~UAsset();

	UPROPERTY(EditAnywhere, Category = "Sequencer")
	class ALevelSequenceActor * Sequencer;

	class UWorld * World;

	FString SceneName;
	FString ContentName;
	FString CutName;
	FString Type;

private:

public:


	virtual void Init(ALevelSequenceActor *CurrentSequencer, FString NewSceneName);

	class AActor * GetActor();

	FGuid GetGuid();

	virtual bool Load();

	virtual bool Unload();

	virtual bool IsLoaded();

};
