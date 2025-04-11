// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class SSceneAssetViewerWidget : public SCompoundWidget 
{
public:
	SLATE_BEGIN_ARGS(SSceneAssetViewerWidget){}
	SLATE_END_ARGS()

	void Construct(const FArguments& inArgs);
private:

	void PopulateAssetSet();
	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FString> item, const TSharedRef<STableViewBase>& OwnerTable);

	TSet<FString> BlueprintSet;
	TSet<FString> MeshSet;
	TSet<FString> MatSet;
	TArray<TSharedPtr<FString>> AssetsList;

	TSharedPtr<SListView<TSharedPtr<FString>>> ListViewWidget;

};
