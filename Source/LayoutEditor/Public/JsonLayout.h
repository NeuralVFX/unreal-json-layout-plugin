// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dom/JsonObject.h"
#include "JsonObjectConverter.h"
#include "JsonLayout.generated.h"


typedef TSharedPtr<FJsonObject> JsonObjectPtr;

/** Struct representing StaticMesh in scene*/
USTRUCT()
struct FStaticMeshObject
{
	GENERATED_BODY()

public:

	/** Name asset will be given in scenefile */
	UPROPERTY()
	FString SceneName;

	/** Content library path for asset */
	UPROPERTY()
	FString ContentName;

	/** Transform of object in scene */
	UPROPERTY()
	FMatrix Matrix;
};

/** Struct representing animated StaticMesh in scene*/
USTRUCT()
struct FAnimatedStaticMeshObject
{
	GENERATED_BODY()

public:

	// Name asset will be given in scenefile
	UPROPERTY()
	FString SceneName;

	// Content library path for asset
	UPROPERTY()
	FString ContentName;

	// Array of keyframes for translation, rotation and scale
	UPROPERTY()
	TArray<float> frame;

	UPROPERTY()
	TArray<float> tx;

	UPROPERTY()
	TArray<float> ty;

	UPROPERTY()
	TArray<float> tz;

	UPROPERTY()
	TArray<float> rx;

	UPROPERTY()		
	TArray<float> ry;

	UPROPERTY()
	TArray<float> rz;

	UPROPERTY()
	TArray<float> sx;

	UPROPERTY()
	TArray<float> sy;

	UPROPERTY()
	TArray<float> sz;
};


USTRUCT()
struct FStaticMeshArray
{
	GENERATED_BODY()

public:

	// Array storing all static mesh structs
	UPROPERTY()
	TArray<FStaticMeshObject> StaticMeshes;

	// Array storing all animated static mesh structs
	UPROPERTY()
	TArray<FAnimatedStaticMeshObject> AnimatedStaticMeshes;
};


UCLASS()
class LAYOUTEDITOR_API UJsonLayout : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	 * Write JSON file describing layout of scene.
	 * @param LayoutName - Name for the JSON file.
	 * @param ActorArray - Array of actors which will be writtin into JSON.
	 * @param Sequencer - Sequencer which contains any keyframes you want stored.
	 * @return Whether operation is successfull.
	 */
	UFUNCTION(BlueprintCallable, Category = "Json")
	static bool WriteLayoutData(FString LayoutName, TArray<AStaticMeshActor*> ActorArray, ALevelSequenceActor * Sequencer);
	
	/**
	 * Read JSON file describing layout of scene.
	 * @param LayoutName - Name for the JSON file.
     * @return Struct containing information from JSON file.
	 */
	static FStaticMeshArray ReadLayoutData(FString LayoutName);

	/**
	 * Create struct with Actor layout information.
	 * @param Actor - Actor in scene.
	 * @return Struct containing object transfom.
	 */
	static FStaticMeshObject MakeStaticMesh(AStaticMeshActor* Actor);

	/**
	 * Create struct with Actor animation information.
	 * @param Sequencer - Sequence Actor with objects animation.
	 * @param Actor - Actor in scene.
	 * @param Guid - Guid of object in supplied sequencer.
	 * @return Struct containing objects animation curves.
	 */
	static FAnimatedStaticMeshObject MakeAnimatedStaticMesh(class ALevelSequenceActor* Sequencer, AStaticMeshActor* Actor, FGuid Guid);

};
