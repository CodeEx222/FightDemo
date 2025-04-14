// 


#include "TimeLineDraw.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
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


	const double GameTime = UGameplayStatics::GetTimeSeconds(this);
	auto currentPosX = CurrentTimeEndPos.X;

	// 100 像素为 1秒 所以1个像素为10毫秒

	auto StartGameSecond = GameTime - CurrentTimeEndPos.X*10/1000.0;
	// StartGameSecond 为开始绘制的时间，现在要找到每隔100毫秒也就是10像素的电，
	// 所以要找到 大于 StartGameSecond 的 整数0.1 的值


	for (int i = 0; i < UiSize.X; i+=10)
	{

	}




	return ResiltId;
}
