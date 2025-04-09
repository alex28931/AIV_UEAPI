// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuickAction.h"
#include "ContentBrowserModule.h"
#include "Styling/SlateStyle.h"           
#include "Styling/SlateStyleRegistry.h"                
#include "SlateCore.h"
#include "../HeaderDebug.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetToolsModule.h"
#include "AssetViewUtils.h"
#include "SExSplineInstantiatorWidget.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "SlateWidget/MyCustomWidget.h"

#define LOCTEXT_NAMESPACE "FQuickActionModule"
DEFINE_LOG_CATEGORY(LogQuickActions);

void FQuickActionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	InitCBExtension();
	RegisterSlateTab();
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
		SelectedFolderPaths = SelectedPaths;
	}
	return MenuExtender;
}

void FQuickActionModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Delete UnusedAsset")),
		FText::FromString(TEXT("Delete UnusedAsset in folder")),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Delete"),
		FExecuteAction::CreateRaw(this, &FQuickActionModule::OnDeleteUnusedAssetClicked)
	);

	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Spawn test tab")),
		FText::FromString(TEXT("Slate test nomad spawn")),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"),
		FExecuteAction::CreateRaw(this, &FQuickActionModule::OnSpawnTabClicked)
	);
}

void FQuickActionModule::OnDeleteUnusedAssetClicked()
{
	if (SelectedFolderPaths.Num() > 1)
	{
		DebugHeader::ShowDialog(EAppMsgType::Ok, TEXT("Only one folder"));
		return;
	}

	TArray<FString> AssetsPath = UEditorAssetLibrary::ListAssets(SelectedFolderPaths[0]);
	if (AssetsPath.Num() == 0)
	{
		DebugHeader::ShowDialog(EAppMsgType::Ok, TEXT("Empty folder"));
		return;
	}

	if (DebugHeader::ShowDialog(EAppMsgType::YesNo, TEXT("Found ") + FString::FromInt(AssetsPath.Num()) + TEXT(" assets\nProceed?")) == EAppReturnType::No)
	{
		return;
	}

	FixRedirectors();
	TArray<FAssetData> UnusedAssets;
	for (const FString& path : AssetsPath)
	{
		if (path.Contains(TEXT("Developers")) || path.Contains(TEXT("Collections"))) 
		{
			continue;
		}
		if (!UEditorAssetLibrary::DoesAssetExist(path))
		{
			continue;
		}
		if (UEditorAssetLibrary::FindPackageReferencersForAsset(path).IsEmpty())
		{
			const FAssetData unusedData = UEditorAssetLibrary::FindAssetData(path);
			UnusedAssets.Add(unusedData);
		}
	}
	if (UnusedAssets.Num() > 0)
	{
		const int32 deleteObjectsNum = ObjectTools::DeleteAssets(UnusedAssets);
		if (deleteObjectsNum == 0) return;
		DebugHeader::ShowNotifyInfo(TEXT("Succesfully deleted " + FString::FromInt(deleteObjectsNum) + " unused assets"));
	}
	else
	{
		DebugHeader::ShowDialog(EAppMsgType::Ok, TEXT("No unused assets"));
	}
}
void FQuickActionModule::RegisterPngIcons(const FString& StyleSetName, const TMap<FString, FString>& IconMap, const FString& IconFolderPath)
{
    const ISlateStyle* ExistingStyle = FSlateStyleRegistry::FindSlateStyle(FName(*StyleSetName));
    if (!ExistingStyle)
    {
        TSharedRef<FSlateStyleSet> NewStyleSet = MakeShareable(new FSlateStyleSet(FName(*StyleSetName)));
        NewStyleSet->SetContentRoot(IconFolderPath);

        for (const auto& IconPair : IconMap)
        {
            const FString& IconName = IconPair.Key;
            const FString& IconFileName = IconPair.Value;

            NewStyleSet->Set(*IconName, new FSlateImageBrush(
                NewStyleSet->RootToContentDir(IconFileName, TEXT(".png")),
                FVector2D(16.0f, 16.0f)
            ));
        }

        // Registra il nuovo stile
        FSlateStyleRegistry::RegisterSlateStyle(*NewStyleSet);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Style '%s' already exists. No changes made."), *StyleSetName);
    }
}
void FQuickActionModule::FixRedirectors()
{
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
	TArray<FAssetData> AssetList;

	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Emplace("/Game");
	Filter.ClassPaths.Add(UObjectRedirector::StaticClass()->GetClassPathName());

	AssetRegistry.GetAssets(Filter, AssetList);

	if (AssetList.Num() == 0) return;

	TArray<FString> ObjectPaths;

	for (const FAssetData& data : AssetList)
	{
		ObjectPaths.Add(data.GetObjectPathString());
	}

	TArray<UObject*> Objects;
	bool Result = AssetViewUtils::LoadAssetsIfNeeded(ObjectPaths, Objects, true, true);
	if (Result)
	{
		TArray<UObjectRedirector*> Redirectors;
		for (UObject* object : Objects)
		{
			Redirectors.Add(CastChecked<UObjectRedirector>(object));
		}

		IAssetTools& AssetTool = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools")).Get();
		AssetTool.FixupReferencers(Redirectors);
	}
}
#pragma endregion

#pragma region SlateTest
void FQuickActionModule::RegisterSlateTab()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("TestTab"), FOnSpawnTab::CreateRaw(this, &FQuickActionModule::OnSpawnTab)).SetDisplayName(FText::FromString(TEXT("Nomad tab test"))
		);
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("SplineSpawnerTab"), FOnSpawnTab::CreateLambda([](const FSpawnTabArgs& Arg)
		{
			return SNew(SDockTab).TabRole(ETabRole::NomadTab)
				[
					SNew(SExSplineInstantiatorWidget)
				];
		}));
}
void FQuickActionModule::OnSpawnTabClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(FName("TestTab"));
}
TSharedRef<SDockTab> FQuickActionModule::OnSpawnTab(const FSpawnTabArgs& args)
{
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SNew(SMyCustomWidget).TestString(TEXT("Test Data passing"))
		];
}
#pragma endregion
void FQuickActionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuickActionModule, QuickAction)