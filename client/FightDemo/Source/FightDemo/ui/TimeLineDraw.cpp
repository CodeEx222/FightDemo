// 


#include "TimeLineDraw.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "FightDemo/fight/AnimDefine.h"
#include "FightDemo/fight/FightInfo.h"
#include "Kismet/GameplayStatics.h"


UTimeLineDraw::UTimeLineDraw(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{


}

void UTimeLineDraw::NativeConstruct()
{
	Super::NativeConstruct();

}

void UTimeLineDraw::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (UiSize.Equals(FVector2D::Zero()))
	{
		FGeometry cachedGeometry = this->GetCachedGeometry();
		UiSize = cachedGeometry.GetLocalSize();

		CurrentTimeStartPos = FVector2D(UiSize.X/3, 0);
		CurrentTimeEndPos = FVector2D(UiSize.X/3, UiSize.Y);
	}


}

int32 UTimeLineDraw::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                 const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                 const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	auto ResiltId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                          bParentEnabled);

	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// 绘制外框
	UWidgetBlueprintLibrary::DrawLine(Context,FVector2D {0,0}, FVector2D {UiSize.X,0});
	UWidgetBlueprintLibrary::DrawLine(Context,FVector2D {0,0}, FVector2D {0, UiSize.Y});
	UWidgetBlueprintLibrary::DrawLine(Context,FVector2D {UiSize.X,0}, FVector2D {UiSize.X, UiSize.Y});
	UWidgetBlueprintLibrary::DrawLine(Context,FVector2D {0, UiSize.Y}, FVector2D {UiSize.X, UiSize.Y});

	UWidgetBlueprintLibrary::DrawLine(Context,
		CurrentTimeStartPos, CurrentTimeEndPos, FLinearColor::Red, true, 1.0f);


	auto GameTime = UGameplayStatics::GetTimeSeconds(this);
	auto currentPosX = CurrentTimeEndPos.X;

	// 10 像素为 1秒 所以1个像素为100毫秒
	auto posRate = 100;
	// 起始时间
	auto startGameMillSecond = static_cast<int>(GameTime * 1000 - currentPosX) ;

	// 起始偏移位移
	auto startUnit = startGameMillSecond % 1000;


	// StartGameSecond 为开始绘制的时间，现在要找到每隔100毫秒也就是10像素的电，
	// 所以要找到 大于 StartGameSecond 的 整数0.1 的值 每隔 100 像素 1 秒 画一个线
	for (int i = startUnit; i < UiSize.X; i+=1000)
	{

		auto drawPointStart = FVector2D(i, UiSize.Y- 15);
		auto drawPointEnd = FVector2D(i, UiSize.Y- 10);

		UWidgetBlueprintLibrary::DrawLine(Context,
		drawPointStart, drawPointEnd, FLinearColor::White, true, 3.0f);

		UWidgetBlueprintLibrary::DrawText(Context, FString::Printf(TEXT("%d"), i / 1000),
			drawPointEnd);
	}




	return ResiltId;
}

void UTimeLineDraw::SetPlaySkillPoint(double totalTime, const TArray<FAnimNotifyEvent>& NotifyArray)
{
	StartTime = UGameplayStatics::GetTimeSeconds(this);
	TotalTime = totalTime;

	for (const auto& notify : NotifyArray)
	{
		if (notify.Notify != nullptr)
		{
			if (const auto NotifyState = Cast<UFightAnimNotify>(notify.Notify); NotifyState != nullptr)
			{
				const auto NotifyTime = notify.GetTime(EAnimLinkMethod::Relative);
				auto notifyEnum = NotifyState->AnimEnum;

				auto DrawPoint = FSkillDrawPoint();
				DrawPoint.StartTime = NotifyTime;
				DrawPoint.EndTime = -1;
				DrawPoint.ShowText = NotifyState->GetNotifyName();

				AllDarDrawPoints.Add(DrawPoint);
			}
		}
		else if (notify.NotifyStateClass != nullptr)
		{
			if (const auto NotifyState = Cast<UFightAnimNotifyState>(notify.NotifyStateClass); NotifyState != nullptr)
			{
				const auto NotifyTime = notify.GetTime(EAnimLinkMethod::Relative);
				auto notifyEnum = NotifyState->AnimEnum;

				auto DrawPoint = FSkillDrawPoint();
				DrawPoint.StartTime = NotifyTime;
				DrawPoint.EndTime = -1;
				DrawPoint.ShowText = NotifyState->GetNotifyName();

				AllDarDrawPoints.Add(DrawPoint);
			}
		}
	}

}

