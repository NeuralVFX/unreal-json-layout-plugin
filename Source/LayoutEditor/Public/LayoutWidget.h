// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "LayoutWidget.generated.h"

/** GUI for JSON layout tool */
UCLASS(BlueprintType)
class LAYOUTEDITOR_API ULayoutWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:

	ULayoutWidget();
	~ULayoutWidget();

	/** GUI objects */
	class UVerticalBox* VerticalBox;

	/** Storage array for asset objects loaded from JSON file */
	TArray<class UAssetBox*> AssetBoxArray;

public:

	/**
	 * Save layout of selected objects as JSON file.
	 */
	UFUNCTION(BlueprintCallable)
	void SaveJson();

	/**
	 * Load layout from JSON file, brings up window to select JSON file.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadJson();

	/**
	 * Loop through and load all object in layout, update GUI.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadAll();

	/**
	 * Loop through and unload all object in layout, update GUI.
	 */
	UFUNCTION(BlueprintCallable)
	void UnLoadAll();

	/**
	 * Opens GUI to select which JSON file to load.
	 * @return Filename of selected file.
	 */
	UFUNCTION(BlueprintCallable)
	FString LoadJsonFile();

	/**
	 * Opens GUI to type JSON filename.
	 * @return Filename which has been typed or selected.
	 */
	UFUNCTION(BlueprintCallable)
	FString SaveJsonFile();

	/**
	 * Find the Sequencer in the scene file.
	 * @return Sequencer object.
	 */
	UFUNCTION(BlueprintCallable)
	class ALevelSequenceActor* GetSequenceActor();

protected:

	/**
	 * Build Gui
	 */
	 virtual void NativePreConstruct() override;

};

