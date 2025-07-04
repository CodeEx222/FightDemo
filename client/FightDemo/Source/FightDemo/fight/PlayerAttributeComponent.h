// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAttributeComponent.generated.h"

class UHeadView;

USTRUCT(BlueprintType)
struct FCharacterAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "当前值"))
	double Value = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "最大值"))
	double MaxValue = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "回复速度"))
	double RecoverValue = 20.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "回复开始时间"))
	double RecoverTime = 4.0;

};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIGHTDEMO_API UPlayerAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Pointer to the life bar widget */
	UPROPERTY()
	TWeakObjectPtr<UHeadView> HeadViewWidget;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "血量"))
	FCharacterAbility HPValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "格挡值"))
	FCharacterAbility BlockValue;

	void ChangeHpValue(double Value);
	void ChangeBlockValue(double Value, bool bIsSetTime = true);

private:

	void GetHeadViewWidget();


	double LastChangeBlockTime = 0.0;
};
