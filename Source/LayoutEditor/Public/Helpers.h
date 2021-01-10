// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Helpers.generated.h"

/**
 * 
 */
UCLASS()
class LAYOUTEDITOR_API UHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

		static class UTextBlock *MakeTextBlock(FString Text, int Size, FLinearColor Color);
	
		static void UpdateSequencer(ALevelSequenceActor* CurrentSequencer);

		static const void * GetParentWindow();

		static FFrameNumber GetFrameNumberTick(ALevelSequenceActor* CurrentSequencer,int Frame, bool Reverse);

		static class ULevelSequence * GetSequence(ALevelSequenceActor* CurrentSequencer);

		static FGuid GetGuidFromSequencer(ALevelSequenceActor* CurrentSequencer, FString SceneName);

		static AActor* GetActorFromSequencer(ALevelSequenceActor* CurrentSequencer, FGuid Guid);

		static class UMovieScene3DTransformSection* GetTransformSection(ALevelSequenceActor* CurrentSequencer, FGuid Guid);

		static TArray<struct FMovieSceneFloatValue> MakeKeyArray(TArray<float> ValueArray);


};
