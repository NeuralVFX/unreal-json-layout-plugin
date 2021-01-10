// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HorizontalBox.h"
#include "AssetBox.generated.h"

/**
 * 
 */
UCLASS()
class LAYOUTEDITOR_API UAssetBox : public UHorizontalBox
{
	GENERATED_BODY()
public:

	class UAsset* Asset;

	class UCheckBox * LoadButton;

	class UTextBlock* ContentNameText;

	class UTextBlock* SceneNameText;

	class UTextBlock* ObjectTypeText;
	
	void Init(UAsset*InAsset);

	UFUNCTION()
	void OnStateChanged(bool NewState);
	
	UFUNCTION()
	void EvaluateState();

};

