// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakObjectPtrTemplatesFwd.h"
#include "Widgets/SCompoundWidget.h"

class AActor;
class USplineComponent;

/**
 * 
 */
class QUICKACTION_API SExSplineInstantiatorWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SExSplineInstantiatorWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SWidget> MakeActorPicker();
	TSharedRef<SWidget> MakeSplinePicker();
	FReply OnSpawnButtonClick();

	TWeakObjectPtr<AActor> SelectedActor;
	TWeakObjectPtr<USplineComponent> SelectedSpline;
	int32 CopiesNumber = 5;
};
