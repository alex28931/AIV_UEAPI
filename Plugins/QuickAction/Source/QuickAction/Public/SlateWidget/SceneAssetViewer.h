// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
struct ListAsset
{
	FString Type;
	FString Path;
	ListAsset(FString type, FString path) : Type(type), Path(path) {}
};
class SSceneAssetViewerWidget : public SCompoundWidget 
{
public:
	SLATE_BEGIN_ARGS(SSceneAssetViewerWidget){}
	SLATE_END_ARGS()

	void Construct(const FArguments& inArgs);
private:

	void PopulateAssetSet();
	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<ListAsset> item, const TSharedRef<STableViewBase>& OwnerTable);

	TSet<FString> BlueprintSet;
	TSet<FString> MeshSet;
	TSet<FString> MatSet;
	TArray<TSharedPtr<ListAsset>> AssetsList;

	TSharedPtr<SListView<TSharedPtr<ListAsset>>> ListViewWidget;

};
