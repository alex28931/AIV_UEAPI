// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"

#include "SplineInstantiatorWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct QUICKACTION_API FSplineInstantiatorStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FSplineInstantiatorStyle();
	virtual ~FSplineInstantiatorStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FSplineInstantiatorStyle& GetDefault();
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class USplineInstantiatorWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FSplineInstantiatorStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
