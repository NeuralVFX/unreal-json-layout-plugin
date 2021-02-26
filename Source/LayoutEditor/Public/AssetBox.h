// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "Components/HorizontalBox.h"
#include "AssetBox.generated.h"

/** UI Component Which Manages Loading and Unloading of Assets */
UCLASS()
class LAYOUTEDITOR_API UAssetBox : public UHorizontalBox
{
	GENERATED_BODY()
public:
	
	/** Actual Asset */
	UPROPERTY()
	class UAsset* Asset;

	/** Load/Unload CheckBox */
	class UCheckBox* LoadButton;
	
	/** Text Blocks */
	class UTextBlock* ContentNameText;

	class UTextBlock* SceneNameText;

	class UTextBlock* ObjectTypeText;
	
public:

	/**
	* Initiate the variables of the AssetBox.
	* @param InAsset - Asset object which this GUI component will control.
	*/
	void Init(UAsset*InAsset);

	/**
	* CheckBox state change, loads and unloads the asset.
	* @param NewState - Whether we want to load or unload.
	*/
	UFUNCTION()
	void OnStateChanged(bool NewState);
	
	/**
	* Checks the scene file to determine the actual state the asset.
	*/
	UFUNCTION()
	void EvaluateState();

};

