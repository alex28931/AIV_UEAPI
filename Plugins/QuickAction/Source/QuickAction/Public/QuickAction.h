// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogQuickActions, Log, All);

class FQuickActionModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private: 

#pragma region CBExtension
	void InitCBExtension();
	TSharedRef<FExtender> CustomCBExtender(const TArray<FString>& SelectedPaths);
	void AddCBMenuEntry(FMenuBuilder& MenuBuilder);
	void OnDeleteUnusedAssetClicked();
#pragma endregion
};
