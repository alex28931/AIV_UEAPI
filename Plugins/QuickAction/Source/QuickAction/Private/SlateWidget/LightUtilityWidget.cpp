// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidget/LightUtilityWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Engine/PointLight.h"
#include "Engine/SpotLight.h"
#include "Engine/RectLight.h"

void SLishtUtilityWidget::Construct(const FArguments& inArgs)
{
}

void SLishtUtilityWidget::RefreshLights()
{
}

TSharedRef<ITableRow> SLishtUtilityWidget::OnGenerateLightRow(TSharedPtr<FLightItem> item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return TSharedRef<ITableRow>();
}

FReply SLishtUtilityWidget::OnRowClicked(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

TSharedRef<SWidget> SLishtUtilityWidget::BuildLightControls(LightType Type)
{
	return TSharedRef<SWidget>();
}