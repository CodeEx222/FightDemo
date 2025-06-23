// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeadView.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTDEMO_API UHeadView : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Sets the life bar to the provided 0-1 percentage value*/
	UFUNCTION(BlueprintImplementableEvent, Category="LifeBar")
	void SetHpPercentage(float Percent);

	/** Sets the life bar to the provided 0-1 percentage value*/
	UFUNCTION(BlueprintImplementableEvent, Category="LifeBar")
	void SetBlockPercentage(float Percent);

};
