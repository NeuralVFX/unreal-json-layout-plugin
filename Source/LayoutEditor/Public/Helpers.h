// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Helpers.generated.h"

/** Collection of techincal utilites to help with GUI and Sequencer logic */
UCLASS()
class LAYOUTEDITOR_API UHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	* Make a TextBlock and set the color and size.
	* @param Text - Text to display.
	* @param Size - Font size.
	* @param Color - Color to assign.
	* @return TextBlock object.
	*/
	static class UTextBlock *MakeTextBlock(FString Text, int Size, FLinearColor Color);
	
	/**
	* Update GUI for sequencer.
	* @param  CurrentSequencer - LevelSequenceActor to update.
	*/
	static void UpdateSequencer(ALevelSequenceActor* CurrentSequencer);

	/**
	* Get Unreal parent window for GUI operations.
	* @return GUI parent window object.
	*/
	static const void * GetParentWindow();

	/**
	* Update GUI for sequencer.
	* @param  CurrentSequencer - LevelSequenceActor to query.
	* @param  Frame - Frame number to query.
	* @param  Reverse - Whether to convert from Tick Resolution to Display Rate or visa-versa.
	*/
	static FFrameNumber GetFrameNumberTick(ALevelSequenceActor* CurrentSequencer, int Frame, bool Reverse);

	/**
	* Retrieve LevelSequence from LevelSequenceActor.
	* @param  CurrentSequencer - LevelSequenceActor to query.
	* @return LevelSequence found in supplied LevelSequenceActor.
	*/
	static class ULevelSequence * GetSequence(ALevelSequenceActor* CurrentSequencer);

	/**
	* Query the Sequencer Guid of a specific object.
	* @param  CurrentSequencer - LevelSequenceActor to query.
	* @param  SceneName - Name of object to search for.
	* @return Guid found for supplied name.
	*/
	static FGuid GetGuidFromSequencer(ALevelSequenceActor* CurrentSequencer, FString SceneName);

	/**
	* Retrieve Actor associated with specific Guid.
	* @param  CurrentSequencer - LevelSequenceActor to query.
	* @param  Guid - Guid of object to search for.
	* @return Actor found for supplied Guid.
	*/
	static AActor* GetActorFromSequencer(ALevelSequenceActor* CurrentSequencer, FGuid Guid);

	/**
	* Retrieve Sequencer Transform Section associated with specific Guid.
	* @param  CurrentSequencer - LevelSequenceActor to query.
	* @param  Guid - Guid of object to search for.
	* @return Actor found for supplied Guid.
	*/
	static class UMovieScene3DTransformSection* GetTransformSection(ALevelSequenceActor* CurrentSequencer, FGuid Guid);

	/**
	* Create an array of MovieScene values from a float array.
	* @param  ValueArray - Array of float values.
	* @return Array of MovieScene values.
	*/
	static TArray<struct FMovieSceneFloatValue> MakeKeyArray(TArray<float> ValueArray);

	/**
	* Convert objects content path into a short GUI friendly string.
	* @param  ContentPath - Full content path.
	* @return Shortened UI friendly content path.
	*/
	static FString MakePrettyContentName(FString ContentPath);

};
