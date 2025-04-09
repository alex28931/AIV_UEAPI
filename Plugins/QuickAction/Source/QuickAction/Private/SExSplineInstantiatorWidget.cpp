// Fill out your copyright notice in the Description page of Project Settings.


#include "SExSplineInstantiatorWidget.h"
#include "Components/SplineComponent.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "PropertyCustomizationHelpers.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SExSplineInstantiatorWidget::Construct(const FArguments& InArgs)
{

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().AutoWidth()
					[
						SNew(STextBlock).Text(FText::FromString("Copies"))
					]
					+ SHorizontalBox::Slot().FillWidth(1.0f)
					[
						SNew(SNumericEntryBox<int32>).MinValue(2).MaxValue(20)
							.Value_Lambda([this]()
								{
									return CopiesNumber;
							})
							.OnValueChanged_Lambda([this](int32 newNumber)
								{
									CopiesNumber = newNumber;
								})
					]

			]
		+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(SExpandableArea).HeaderContent()
					[
						SNew(STextBlock).Text(FText::FromString("Select Actor"))
					].BodyContent()
							[
								MakeActorPicker()
							]
			]
		+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(SExpandableArea).HeaderContent()
					[
						SNew(STextBlock).Text(FText::FromString("Select Spline"))
					].BodyContent()
							[
								MakeSplinePicker()
							]
			]
		+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(SButton).Text(FText::FromString("Spawn actor along spline")).OnClicked(this, &SExSplineInstantiatorWidget::OnSpawnButtonClick)
			]
	];

}
TSharedRef<SWidget> SExSplineInstantiatorWidget::MakeActorPicker()
{
	return PropertyCustomizationHelpers::MakeActorPickerWithMenu
	(
		nullptr, 
		false, 
		FOnShouldFilterActor::CreateLambda(
			[](const AActor* actor)
			{
				if (!actor) return false;
				return true;
			})
		,
		FOnActorSelected::CreateLambda(
			[this](AActor* actor)
			{
				SelectedActor = actor;
			}
		)
		,
		FSimpleDelegate::CreateLambda(
			[&]() -> void
			{
				
			}
		)
		,
		FSimpleDelegate::CreateLambda(
			[&]() -> void
			{

			}
		)
	);
}

TSharedRef<SWidget> SExSplineInstantiatorWidget::MakeSplinePicker()
{
	return PropertyCustomizationHelpers::MakeActorPickerWithMenu
	(
		nullptr,
		false,
		FOnShouldFilterActor::CreateLambda(
			[](const AActor* actor)
			{
				if (!actor) return false;
				if (!actor->FindComponentByClass<USplineComponent>()) return false;
				return true;
			})
		,
		FOnActorSelected::CreateLambda(
			[this](AActor* actor)
			{
				SelectedSpline = actor->FindComponentByClass<USplineComponent>();
			}
		)
		,
		FSimpleDelegate::CreateLambda(
			[&]() -> void
			{

			}
		)
		,
		FSimpleDelegate::CreateLambda(
			[&]() -> void
			{

			}
		)
	);
}

FReply SExSplineInstantiatorWidget::OnSpawnButtonClick()
{
	if (!SelectedActor.IsValid() || !SelectedSpline.IsValid()) return FReply::Handled();

	UWorld* CurrentWorld = GEditor->GetEditorWorldContext().World();
	if (!CurrentWorld) return FReply::Handled();

	for (int32 i = 0; i < CopiesNumber; i++)
	{
		float Alpha = (float)i / (float)(CopiesNumber - 1);

		float TotalLength = SelectedSpline->GetSplineLength();

		float Distance = Alpha * TotalLength;

		FVector spawnedActorLocation = SelectedSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		FRotator spawnedActorRotation = SelectedSpline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		FVector spawnedActorScale = SelectedSpline->GetScaleAtDistanceAlongSpline(Distance);

		AActor* newActor = CurrentWorld->SpawnActor<AActor>(SelectedActor->GetClass(), spawnedActorLocation, spawnedActorRotation);
		if (newActor)
		{
			newActor->SetActorScale3D(spawnedActorScale);
		}
	}

	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
