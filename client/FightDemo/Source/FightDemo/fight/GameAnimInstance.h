#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "mode/FightInstance.h"
#include "GameAnimInstance.generated.h"

class AGameFightBase;
struct FMontageBlendSettings;
class AGameFightCharacter;
/**
 * 
 */
UCLASS()
class FIGHTDEMO_API UGameAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	AGameFightBase* gameCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "左右移动值"))
	float MoveActionX = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "前后移动值"))
	float MoveActionY = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EHitDirection8 MoveDirection = EHitDirection8::None;


	UFUNCTION(BlueprintCallable)
	void CalMoveDirByVelocity();

	UFUNCTION()
	float PlayAnimMontage(class UAnimMontage* AnimMontage,
		float InPlayRate = 1.0f, FName StartSectionName = NAME_None,
		EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength);

	float PlayAnimSequenceByPath(const FString& AnimPath, FName SlotNodeName,
		float BlendInTime = 0.1f, float BlendOutTime =0.1f,
		float InPlayRate = 1.f, int32 LoopCount = 1, float InBlendOutTriggerTime = -1.f);

	UFUNCTION(BlueprintCallable)
	float PlayAnimSequence(UAnimSequence* AnimSequence, FName SlotNodeName,
		float BlendInTime = 0.1f, float BlendOutTime =0.1f,
		float InPlayRate = 1.f, int32 LoopCount = 1, float InBlendOutTriggerTime = -1.f);

private:
	float PlayAnimationDirectly(UAnimSequence* AnimSequence, FName SlotNodeName,
		const FMontageBlendSettings& BlendInSettings, const FMontageBlendSettings& BlendOutSettings,
		float InPlayRate = 1.f, int32 LoopCount = 1, float InBlendOutTriggerTime = -1.f);

};
