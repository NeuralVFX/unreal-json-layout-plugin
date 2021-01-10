// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "LayoutWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class LAYOUTEDITOR_API ULayoutWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	ULayoutWidget();
	~ULayoutWidget();

	class UVerticalBox* VerticalBox;
	class UVerticalBox* JsonVerticalBox;



	TArray<class UAssetBox*> AssetBoxArray;
	TArray<class UAsset*> AssetArray;



	UFUNCTION(BlueprintCallable)
		void SaveJson();

	UFUNCTION(BlueprintCallable)
		void LoadJson();

	UFUNCTION(BlueprintCallable)
		void LoadAll();

	UFUNCTION(BlueprintCallable)
		void UnLoadAll();

	UFUNCTION(BlueprintCallable)
		FString LoadJsonFile();

	UFUNCTION(BlueprintCallable)
		FString SaveJsonFile();

	UFUNCTION(BlueprintCallable)
		class ALevelSequenceActor* GetSequenceActor();

protected:

	 virtual void NativePreConstruct() override;
};

