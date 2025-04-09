// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineInstantiatorWidgetStyle.h"

FSplineInstantiatorStyle::FSplineInstantiatorStyle()
{
}

FSplineInstantiatorStyle::~FSplineInstantiatorStyle()
{
}

const FName FSplineInstantiatorStyle::TypeName(TEXT("FSplineInstantiatorStyle"));

const FSplineInstantiatorStyle& FSplineInstantiatorStyle::GetDefault()
{
	static FSplineInstantiatorStyle Default;
	return Default;
}

void FSplineInstantiatorStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

