// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SizeBox.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	if (!IsValid(AttachedActor)) // IsValid checks for both null and IsPendingKill
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing Health Widget."));

		return;
	}

	// every tick just project the world location since this is the base class

	FVector2D ScreenPosition;

	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		// need to also scale this to the viewport scale
		ScreenPosition /= UWidgetLayoutLibrary::GetViewportScale(this);

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}

}
