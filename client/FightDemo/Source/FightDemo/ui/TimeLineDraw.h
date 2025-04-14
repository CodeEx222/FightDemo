// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimeLineDraw.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTDEMO_API UTimeLineDraw : public UUserWidget
{
	GENERATED_BODY()


public:
	UTimeLineDraw(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:

	FVector2D UiSize;


	FVector2D CurrentTimeStartPos;
	FVector2D CurrentTimeEndPos;

};
