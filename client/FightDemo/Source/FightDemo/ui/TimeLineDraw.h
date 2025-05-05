// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimeLineDraw.generated.h"


class UFightAnimNotifyState;
class UFightAnimNotify;

USTRUCT()
struct FSkillDrawPoint
{
	GENERATED_BODY()

public:
	double StartTime;
	double EndTime;

	FString ShowText;


};

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


	void SetPlaySkillPoint(double totalTime,const TArray<FAnimNotifyEvent>& NotifyArray);
private:

	FVector2D UiSize;


	FVector2D CurrentTimeStartPos;
	FVector2D CurrentTimeEndPos;


	// 起始时间
	double StartTime;
	// 总时间
	double TotalTime;

	UPROPERTY()
	TArray<FSkillDrawPoint> AllDarDrawPoints;


};
