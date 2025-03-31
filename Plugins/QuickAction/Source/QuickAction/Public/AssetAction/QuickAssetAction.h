// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "Materials/material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "QuickAssetAction.generated.h"

/**
 * 
 */
UCLASS()
class QUICKACTION_API UQuickAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(CallInEditor)
	void TestAction();

	UFUNCTION(CallInEditor)
	void Duplicate(int32 numCopies);

	UFUNCTION(CallInEditor)
	void FixPrefix();

	UFUNCTION(CallInEditor)
	void FixCustomPrefix(const FString& customPrefix, const FString& customSuffix);

	UFUNCTION(CallInEditor)
	void RemoveUnused();

private: 
	TMap<UClass*, FString> prefixMap =
	{
		{UBlueprint::StaticClass(), TEXT("BP_")},
		{UMaterial::StaticClass(), TEXT("M_")},
		{UMaterialInstanceConstant::StaticClass(), TEXT("MI_")}
	};

	void FixRedirectors();
};
