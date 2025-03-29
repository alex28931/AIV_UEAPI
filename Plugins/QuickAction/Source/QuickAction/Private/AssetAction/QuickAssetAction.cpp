// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetAction/QuickAssetAction.h"
#include "../HeaderDebug.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"

void UQuickAssetAction::TestAction()
{
	ScreenPrint(TEXT("Work as intended"), FColor::Emerald);

	LogPrint(TEXT("Work as intended in output log"));
}

void UQuickAssetAction::Duplicate(int32 numCopies)
{
	TArray<FAssetData> selectedAssetData= UEditorUtilityLibrary::GetSelectedAssetData();
	uint32 counter = 0;
	for (const FAssetData& data : selectedAssetData)
	{
		if (numCopies <= 0)
		{
			ShowDialog(EAppMsgType::Ok, TEXT("Please Enter a valid number (>0)"));
			return;
		}
		for (int32 i = 0; i < numCopies; i++)
		{
			const FString SourcePath = data.ObjectPath.ToString();
			const FString NewAssetName = data.AssetName.ToString() + TEXT("_Mk") + FString::FromInt(i + 1);
			const FString DestinationAssetPath = FPaths::Combine(data.PackagePath.ToString(), NewAssetName);

			if (UEditorAssetLibrary::DuplicateAsset(SourcePath, DestinationAssetPath))
			{
				UEditorAssetLibrary::SaveAsset(DestinationAssetPath, false);
				++counter;
			}
		}
	}
	if (counter > 0)
	{
		ShowNotifyInfo(TEXT("Sucesfully duplicate" + FString::FromInt(counter) + "assets"));
	}

}

void UQuickAssetAction::FixPrefix()
{
	TArray<UObject*> selectedAsset = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 counter = 0;
	for (UObject* obj : selectedAsset)
	{
		if (!obj) continue;
		FString* prefix = prefixMap.Find(obj->GetClass());
		if (!prefix || prefix->IsEmpty()) 
		{
			LogPrint(TEXT("Failed to find prefix for class" + obj->GetClass()->GetName()));
			continue;
		}
		FString oldName = obj->GetName();
		if (oldName.StartsWith(*prefix))
		{
			LogPrint(oldName +TEXT(" Already has valid prefix"));
			continue;
		}
		if (obj->IsA<UMaterialInstanceConstant>())
		{
			oldName.RemoveFromStart("M_");
			oldName.RemoveFromEnd("_Inst");
		}
		const FString newName = *prefix + oldName;
		UEditorUtilityLibrary::RenameAsset(obj, newName);
		++counter;
	}
	if (counter > 0)
	{
		ShowNotifyInfo(TEXT("Sucesfully renamed" + FString::FromInt(counter) + "assets"));
	}
}

void UQuickAssetAction::FixCustomPrefix(const FString& customPrefix, const FString& customSuffix)
{
	TArray<UObject*> selectedAsset = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 counter = 0;
	for (UObject* obj : selectedAsset)
	{
		if (!obj) continue;
		FString* prefix = prefixMap.Find(obj->GetClass());
		if (!prefix || prefix->IsEmpty())
		{
			LogPrint(TEXT("Failed to find prefix for class" + obj->GetClass()->GetName()));
			continue;
		}
		FString oldName = obj->GetName();
		oldName.RemoveFromStart(*prefix);
		oldName.RemoveFromStart(customPrefix+TEXT("_"));
		if (obj->IsA<UMaterialInstanceConstant>())
		{
			oldName.RemoveFromStart("M_");
			oldName.RemoveFromEnd("_Inst");
		}
		oldName.RemoveFromEnd(TEXT("_" + customSuffix));
		FString newName = *prefix;
		if (!customPrefix.IsEmpty())
		{
			newName = newName + (customPrefix + TEXT("_"));
		}
		newName = newName + oldName;
		if (!customSuffix.IsEmpty())
		{
			newName = newName + (TEXT("_") + customSuffix);
		}
		UEditorUtilityLibrary::RenameAsset(obj, newName);
		++counter;
	}
	if (counter > 0)
	{
		ShowNotifyInfo(TEXT("Sucesfully renamed" + FString::FromInt(counter) + "assets"));
	}
}
