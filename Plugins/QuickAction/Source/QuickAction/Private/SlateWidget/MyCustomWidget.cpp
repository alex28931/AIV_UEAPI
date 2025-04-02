// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidget/MyCustomWidget.h"
#include "Widgets/Input/SSpinBox.h"

void SMyCustomWidget::Construct(const FArguments& inArgs)
{
	bCanSupportFocus = true;

	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleFont.Size = 30;

	ChildSlot
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("My Custom Slate Tab")))
						.Font(TitleFont)
						.ColorAndOpacity(FColor::White)
						.Justification(ETextJustify::Center)
				]	
		];
}
