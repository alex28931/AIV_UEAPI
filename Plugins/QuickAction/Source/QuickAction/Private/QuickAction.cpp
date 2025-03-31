// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuickAction.h"
#include "ContentBrowserModule.h"

#define LOCTEXT_NAMESPACE "FQuickActionModule"
DEFINE_LOG_CATEGORY(LogQuickActions);

void FQuickActionModule::StartupModule()
{
	InitCBExtension();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

#pragma region CBExtension

void FQuickActionModule::InitCBExtension()
{
	FContentBrowserModule& CBModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	TArray<FContentBrowserMenuExtender_SelectedPaths>& CBExtenders = CBModule.GetAllPathViewContextMenuExtenders();

	FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;

	CustomCBMenuDelegate.BindRaw(this, &FQuickActionModule::CustomCBExtender);

	CBExtenders.Add(CustomCBMenuDelegate);
}

TSharedRef<FExtender> FQuickActionModule::CustomCBExtender(const TArray<FString>& SelectedPaths)
{
	TSharedRef<FExtender> MenuExtender(new FExtender());
	if (SelectedPaths.Num() > 0)
	{
		MenuExtender->AddMenuExtension(FName("Delete"), EExtensionHook::After, TSharedPtr<FUICommandList>(), FMenuExtensionDelegate::CreateRaw(this, &FQuickActionModule::AddCBMenuEntry));
	}
	return MenuExtender;
}

void FQuickActionModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Delete UnusedAsset")),
		FText::FromString(TEXT("Delete UnusedAsset in folder")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FQuickActionModule::OnDeleteUnusedAssetClicked)
	);
}

void FQuickActionModule::OnDeleteUnusedAssetClicked()
{
}

#pragma endregion
void FQuickActionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuickActionModule, QuickAction)