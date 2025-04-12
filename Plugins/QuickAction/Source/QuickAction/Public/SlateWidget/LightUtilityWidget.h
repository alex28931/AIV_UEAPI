// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
enum class LightType
{
	Point,
	Spot,
	Rect
};

struct FLightItem
{
	FString Name;
	TWeakObjectPtr<ULightComponent> Light;
	LightType Type;
	bool bSelected = false;
};


class SLishtUtilityWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLishtUtilityWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& inArgs);
private:
	void RefreshLights();
	TSharedRef<ITableRow> OnGenerateLightRow(TSharedPtr<FLightItem> item, const TSharedRef<STableViewBase>& OwnerTable);
	FReply OnRowClicked(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	TSharedRef<SWidget> BuildLightControls(LightType Type);

	TArray<TSharedPtr<FLightItem>> PointLightItems;
	TArray<TSharedPtr<FLightItem>> SpotLightItems;
	TArray<TSharedPtr<FLightItem>> AreaLightItems;

	TSharedPtr<SListView<TSharedPtr<FLightItem>>> PointLightListView;
	TSharedPtr<SListView<TSharedPtr<FLightItem>>> SpotLightListView;
	TSharedPtr<SListView<TSharedPtr<FLightItem>>> RectLightListView;
};
