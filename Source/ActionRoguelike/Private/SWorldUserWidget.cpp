// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"
#include <Blueprint/WidgetLayoutLibrary.h>
#include "Components/SizeBox.h"
#include <Kismet/GameplayStatics.h>

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// we need to remove the widget when the attached actor is null or is pending kill (is valid does these both)
	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing Health Widget"));

		return;
	}

	FVector2D ScreenPosition;

	//if(UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation(), ScreenPosition))

	// this fn already takes the scale into account as well
	if(UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition, false))
	{
		//float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		//ScreenPosition /= Scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
