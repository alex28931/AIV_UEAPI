// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidget/MyCustomWidget.h"
#include "Widgets/Input/SSpinBox.h"

void SMyCustomWidget::Construct(const FArguments& inArgs)
{
	bCanSupportFocus = true;

	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleFont.Size = 30;

	//ChildSlot
	//	[
	//		SNew(SVerticalBox)
	//		+SVerticalBox::Slot()
	//			.AutoHeight()
	//			[
	//				SNew(STextBlock)
	//					.Text(FText::FromString(TEXT("My Custom Slate Tab")))
	//					.Font(TitleFont)
	//					.ColorAndOpacity(FColor::White)
	//					.Justification(ETextJustify::Center)
	//			]	
	//		//+SVerticalBox::Slot()
	//		//	.AutoHeight()
	//		//	[
	//		//		SNew(SHorizontalBox)
	//		//		+SHorizontalBox::Slot()
	//		//			[
	//		//				SNew(SSlider)
	//		//					.Value(0.5f)
	//		//					.MinValue(0.0f)
	//		//					.MaxValue(1.0f)
	//		//					.OnValueChanged(HandleSliderValueChange)
	//		//			]
	//		//	]
	//		+SVerticalBox::Slot()
	//			.AutoHeight()
	//			[
	//				SNew(SHorizontalBox)
	//				+ SHorizontalBox::Slot()
	//				[
	//					SNew(const SListView<TSharedPtr<FString>>)
	//						.ListItemsSource(&Option_CB)
	//						.OnGenerateRow(this, &SMyCustomWidget::GenerateListRow)
	//						.OnSelectionChanged(this, &SMyCustomWidget::HandleListSelectionChanged)
	//				]
	//		]
	//	];
}

void SMyCustomWidget::HandleSliderValueChange(float NewValue)
{
	value = NewValue;
	UE_LOG(LogTemp, Log, TEXT("value %f"), value);
}

TSharedRef<ITableRow> SMyCustomWidget::GenerateListRow(TSharedPtr<FString> item, const TSharedRef<STableViewBase> OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(0.1)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("------>")))
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.8)
				//.HAlign(EHorizontalAlignment::HAlign_Center)
				[
					SNew(STextBlock).Text(FText::FromString(*item)).Justification(ETextJustify::Center)
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.1)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("<------")))
				]
		];
}

void SMyCustomWidget::HandleListSelectionChanged(TSharedPtr<FString> selectedItem, ESelectInfo::Type SelectInfo)
{
	if (selectedItem.IsValid())
	{
		DebugHeader::ScreenPrint(*selectedItem, FColor::Orange);
	}
}



