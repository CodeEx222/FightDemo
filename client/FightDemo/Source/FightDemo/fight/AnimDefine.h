#pragma once

#include "CoreMinimal.h"
#include "UObject/UnrealNames.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimDefine.generated.h"


/*
*	// virtual  FString GetNotifyName_Implementation() const override
	// {
	// 	return UEnum::GetDisplayValueAsText(AnimEnum).ToString();
	// };

// #if WITH_EDITOR
// 	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
// 	{
// 		Super::PostEditChangeProperty(PropertyChangedEvent);
//
// 		if (PropertyChangedEvent.GetPropertyName() != GET_MEMBER_NAME_CHECKED(UFightAnimNotify, AnimEnum))
// 		{
// 			return;
// 		}
// 		// 获取动画序列并刷新
// 		if (UAnimMontage* AnimSequence = Cast<UAnimMontage>(GetOuter()))
// 		{
// 			AnimSequence->RefreshCacheData();
// 		}
// 	}
// #endif
 */

UENUM(BlueprintType)
enum class EFightAnimNotify : uint8
{
	None = 0 UMETA(DisplayName="无")  ,
	Attack UMETA(DisplayName="攻击帧")  ,
	AttackNext UMETA(DisplayName="播放下次攻击")  ,

};

UENUM(BlueprintType)
enum class EFightAnimStateNotify : uint8
{
	None = 0 UMETA(DisplayName="无")  ,
	PlayState UMETA(DisplayName="连击有效区间")  ,
	FanJiState UMETA(DisplayName="反击区间")  ,
	InoputState UMETA(DisplayName="输入区间")  ,
	WuDiState UMETA(DisplayName="无敌区间")  ,
};

UCLASS(Blueprintable)
class FIGHTDEMO_API UFightAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFightAnimNotify AnimEnum;

	FString GetNotifyName();

	virtual  FString GetNotifyName_Implementation() const override
	{
		if (Notify_Name.Len() > 0)
		{
			return Notify_Name;
		}
		return Super::GetNotifyName_Implementation();
	};

	virtual void PostInitProperties() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void Notify(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// 通知名字
	UPROPERTY(VisibleAnywhere)
	FString Notify_Name;
private:

	void CalculateValues();

};

UCLASS(Blueprintable)
class FIGHTDEMO_API UFightAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFightAnimStateNotify AnimEnum;

	virtual  FString GetNotifyName_Implementation() const override;

	virtual void PostInitProperties() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// 通知名字
	UPROPERTY(VisibleAnywhere)
	FString Notify_Name;

private:
	void CalculateValues();
};
