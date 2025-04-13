// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidget/LightUtilityWidget.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "Widgets/Colors/SColorBlock.h"
#include "EngineUtils.h"
#include "Components/LightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Editor.h"

void SLightUtilityWidget::Construct(const FArguments& inArgs)
{
	RefreshLights();

	ChildSlot
		[
			SNew(SVerticalBox)

				// Point Lights
				+ SVerticalBox::Slot().AutoHeight().Padding(5)
				[
					SNew(SExpandableArea)
						.AreaTitle(FText::FromString("Point Lights"))
						.BodyContent()
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot().AutoHeight()
								[
									SAssignNew(PointLightListView, SListView<TSharedPtr<FLightItem>>)
										.ItemHeight(24)
										.ListItemsSource(&PointLightItems)
										.OnGenerateRow(this, &SLightUtilityWidget::OnGenerateLightRow)
										.SelectionMode(ESelectionMode::None)
								]
								+ SVerticalBox::Slot().AutoHeight().Padding(5)
								[
									BuildLightControls(LightType::Point)
								]
						]
				]

				// Spot Lights
				+ SVerticalBox::Slot().AutoHeight().Padding(5)
				[
					SNew(SExpandableArea)
						.AreaTitle(FText::FromString("Spot Lights"))
						.BodyContent()
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot().AutoHeight()
								[
									SAssignNew(SpotLightListView, SListView<TSharedPtr<FLightItem>>)
										.ItemHeight(24)
										.ListItemsSource(&SpotLightItems)
										.OnGenerateRow(this, &SLightUtilityWidget::OnGenerateLightRow)
										.SelectionMode(ESelectionMode::None)
								]
								+ SVerticalBox::Slot().AutoHeight().Padding(5)
								[
									BuildLightControls(LightType::Spot)
								]
						]
				]

				// Rect Lights
				+ SVerticalBox::Slot().AutoHeight().Padding(5)
				[
					SNew(SExpandableArea)
						.AreaTitle(FText::FromString("Rect Lights"))
						.BodyContent()
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot().AutoHeight()
								[
									SAssignNew(RectLightListView, SListView<TSharedPtr<FLightItem>>)
										.ItemHeight(24)
										.ListItemsSource(&AreaLightItems)
										.OnGenerateRow(this, &SLightUtilityWidget::OnGenerateLightRow)
										.SelectionMode(ESelectionMode::None)
								]
								+ SVerticalBox::Slot().AutoHeight().Padding(5)
								[
									BuildLightControls(LightType::Rect)
								]
						]
				]
		];
}

void SLightUtilityWidget::RefreshLights()
{
	PointLightItems.Empty();
	SpotLightItems.Empty();
	AreaLightItems.Empty();

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World) return;
	for (TActorIterator<AActor> ActorIt(World); ActorIt; ++ActorIt)
	{
		AActor* Actor = *ActorIt;

		TArray<UActorComponent*> Components;
		Actor->GetComponents(Components);
		for (UActorComponent* Comp : Components)
		{
			if (ULightComponent* LightComp = Cast<ULightComponent>(Comp))
			{
				LightType Type;

				if (Cast<UPointLightComponent>(LightComp))
				{
					Type = LightType::Point;
					if (Cast<USpotLightComponent>(LightComp))
					{
						Type = LightType::Spot;
					}
				}
				else if (Cast<URectLightComponent>(LightComp))
					Type = LightType::Rect;
				else
					continue;

				FLightItem Item;
				Item.Light = LightComp;
				Item.Type = Type;
				FString ActorName = Actor->GetName();
				FString ComponentName = Comp->GetName();
				Item.Name = FString::Printf(TEXT("%s (%s)"), *ActorName, *ComponentName);

				switch (Type)
				{
				case LightType::Point:
					PointLightItems.Add(MakeShared<FLightItem>(Item));
					break;
				case LightType::Spot:
					SpotLightItems.Add(MakeShared<FLightItem>(Item));
					break;
				case LightType::Rect:
					AreaLightItems.Add(MakeShared<FLightItem>(Item));
					break;
				}
			}
		}
	}

	if (PointLightListView.IsValid()) PointLightListView->RequestListRefresh();
	if (SpotLightListView.IsValid()) SpotLightListView->RequestListRefresh();
	if (RectLightListView.IsValid()) RectLightListView->RequestListRefresh();
}

TSharedRef<ITableRow> SLightUtilityWidget::OnGenerateLightRow(TSharedPtr<FLightItem> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FLightItem>>, OwnerTable)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
				[
					SNew(SCheckBox)
						.IsChecked_Lambda([Item]() { return Item->bSelected ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
						.OnCheckStateChanged_Lambda([Item](ECheckBoxState NewState)
							{
								Item->bSelected = (NewState == ECheckBoxState::Checked);
							})
				]
				+ SHorizontalBox::Slot().FillWidth(1).Padding(8, 0)
				[
					SNew(STextBlock)
						.Text(FText::FromString(Item->Name))
				]
		];
}

TSharedRef<SWidget> SLightUtilityWidget::BuildLightControls(LightType Type)
{
	return SNew(SVerticalBox)

		// Sezione per l'intensità
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 4)
		[
			SNew(STextBlock)
				.Text(FText::FromString("Intensity"))
		]
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SSlider)
				.MinValue(0.f)
				.MaxValue(10.f)
				.Value(1.f)
				.OnValueChanged_Lambda([this, Type](float NewValue)
					{
						TArray<TSharedPtr<FLightItem>>* TargetList = nullptr;

						switch (Type)
						{
						case LightType::Point: TargetList = &PointLightItems; break;
						case LightType::Spot:  TargetList = &SpotLightItems;  break;
						case LightType::Rect:  TargetList = &AreaLightItems;  break;
						}

						if (!TargetList) return;

						for (const auto& Item : *TargetList)
						{
							if (Item->bSelected && Item->Light.IsValid())
							{
								Item->Light->SetIntensity(NewValue * 1000.f); // Scala intensità arbitraria
							}
						}
					})
		]
		+ SVerticalBox::Slot().AutoHeight().Padding(0.f,8.f)
		[
			SNew(SHorizontalBox)

				+ SHorizontalBox::Slot().AutoWidth().Padding(2)
				[
					SNew(STextBlock).Text(FText::FromString("R"))
				]
				+ SHorizontalBox::Slot().AutoWidth().Padding(2)
				[
					SNew(SSpinBox<int32>)
						.MinValue(0)
						.MaxValue(255)
						.Value(255)
						.OnValueChanged_Lambda([this, Type](int32 NewR)
							{
								UpdateLightColor(Type, NewR, -1, -1);
							})
				]

				+ SHorizontalBox::Slot().AutoWidth().Padding(2)
				[
					SNew(STextBlock).Text(FText::FromString("G"))
				]
				+ SHorizontalBox::Slot().AutoWidth().Padding(2)
				[
					SNew(SSpinBox<int32>)
						.MinValue(0)
						.MaxValue(255)
						.Value(255)
						.OnValueChanged_Lambda([this, Type](int32 NewG)
							{
								UpdateLightColor(Type, -1, NewG, -1);
							})
				]

				+ SHorizontalBox::Slot().AutoWidth().Padding(2)
				[
					SNew(STextBlock).Text(FText::FromString("B"))
				]
				+ SHorizontalBox::Slot().AutoWidth().Padding(2)
				[
					SNew(SSpinBox<int32>)
						.MinValue(0)
						.MaxValue(255)
						.Value(255)
						.OnValueChanged_Lambda([this, Type](int32 NewB)
							{
								UpdateLightColor(Type, -1, -1, NewB);
							})
				]
		];
}

void SLightUtilityWidget::UpdateLightColor(LightType Type, int32 R, int32 G, int32 B)
{
	TArray<TSharedPtr<FLightItem>>* TargetList = nullptr;

	switch (Type)
	{
	case LightType::Point: TargetList = &PointLightItems; break;
	case LightType::Spot:  TargetList = &SpotLightItems;  break;
	case LightType::Rect:  TargetList = &AreaLightItems;  break;
	}

	if (!TargetList) return;

	for (const auto& Item : *TargetList)
	{
		if (Item->bSelected && Item->Light.IsValid())
		{
			FLinearColor CurrentColor = Item->Light->GetLightColor();
			if (R >= 0) CurrentColor.R = R / 255.0f;
			if (G >= 0) CurrentColor.G = G / 255.0f;
			if (B >= 0) CurrentColor.B = B / 255.0f;

			Item->Light->SetLightColor(CurrentColor);
		}
	}
}

