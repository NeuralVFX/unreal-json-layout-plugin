// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dom/JsonObject.h"
#include "JsonObjectConverter.h"
#include "JsonLayout.generated.h"


typedef TSharedPtr<FJsonObject> JsonObjectPtr;


USTRUCT()
struct FStaticMeshObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FString SceneName;

	UPROPERTY()
		FString ContentName;

	UPROPERTY()
		FMatrix Matrix;
};


USTRUCT()
struct FAnimatedStaticMeshObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FString SceneName;

	UPROPERTY()
		FString ContentName;

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

	UPROPERTY()
		TArray<FStaticMeshObject> StaticMeshes;

	UPROPERTY()
		TArray<FAnimatedStaticMeshObject> AnimatedStaticMeshes;
};


UCLASS()
class LAYOUTEDITOR_API UJsonLayout : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
		UFUNCTION(BlueprintCallable, Category = "Json")
		static bool WriteLayoutData(FString LayoutName, TArray<AStaticMeshActor*> ActorArray, ALevelSequenceActor * Sequencer);
	
		static FStaticMeshArray ReadLayoutData(FString LayoutName);

		static FStaticMeshObject MakeStaticMesh(AStaticMeshActor* Actor);

		static FAnimatedStaticMeshObject MakeAnimatedStaticMesh(class ALevelSequenceActor* Sequencer, AStaticMeshActor* Actor, FGuid Guid);


};
