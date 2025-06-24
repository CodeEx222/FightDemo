


#include "GameAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"
#include "mode/GameFightCharacter.h"



void UGameAnimInstance::SetMoveDirection(float MoveX,float MoveY)
{

	MoveActionX = MoveX;
	MoveActionY = MoveY;
	// 获取移动方向
	if (MoveX == 0.f && MoveY == 0.f)
	{
		MoveDirection = EHitDirection8::None;
	}
	else
	{
		// MoveY >0 表示前进, MoveY <0 表示后退
		// MoveX >0 表示右移, MoveX <0 表示左移
		// 计算角度
		const float AngleDegree = FMath::RadiansToDegrees(FMath::Atan2(MoveX, MoveY));
		// 将角度转换为0-360度范围
		float Angle = FMath::Fmod(AngleDegree + 360.0f, 360.0f);

		if (Angle >= 337.5f || Angle < 22.5f)
		{
			MoveDirection = EHitDirection8::Front;
		}
		else if (Angle >= 22.5f && Angle < 67.5f)
		{
			MoveDirection = EHitDirection8::FrontRight;
		}
		else if (Angle >= 67.5f && Angle < 112.5f)
		{
			MoveDirection = EHitDirection8::Right;
		}
		else if (Angle >= 112.5f && Angle < 157.5f)
		{
			MoveDirection = EHitDirection8::BackRight;
		}
		else if (Angle >= 157.5f && Angle < 202.5f)
		{
			MoveDirection = EHitDirection8::Back;
		}
		else if (Angle >= 202.5f && Angle < 247.5f)
		{
			MoveDirection = EHitDirection8::BackLeft;
		}
		else if (Angle >= 247.5f && Angle < 292.5f)
		{
			MoveDirection = EHitDirection8::Left;
		}
		else if (Angle >= 292.5f && Angle < 337.5f)
		{
			MoveDirection = EHitDirection8::FrontLeft;
		}
	}
}

float UGameAnimInstance::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName,
                                         EMontagePlayReturnType ReturnValueType)
{
	if(AnimMontage)
	{
		float const Duration = this->Montage_Play(AnimMontage, InPlayRate,ReturnValueType);
		if ( Duration > 0.f)
		{
			// Start at a given Section.
			if( StartSectionName != NAME_None )
			{
				this->Montage_JumpToSection(StartSectionName, AnimMontage);
			}

			return Duration;
		}
	}

	return 0.f;
}

float UGameAnimInstance::PlayAnimSequenceByPath(const FString& AnimPath, const FName SlotNodeName,
	const float BlendInTime, const float BlendOutTime,
	const float InPlayRate, const int32 LoopCount, const float InBlendOutTriggerTime)
{
	// 转换路径为软引用
	FSoftObjectPath AnimSequencePathRef = FSoftObjectPath(AnimPath);

	// 同步加载资源
	UAnimSequence* AnimSequence = Cast<UAnimSequence>(
		StaticLoadObject(UAnimSequence::StaticClass(), nullptr, *AnimSequencePathRef.ToString())
	);

	if (AnimSequence)
	{
		FMontageBlendSettings BlendInSettings(BlendInTime);
		FMontageBlendSettings BlendOutSettings(BlendOutTime);
		return PlayAnimationDirectly(AnimSequence, SlotNodeName,
			BlendInSettings,BlendOutSettings,InPlayRate, LoopCount, InBlendOutTriggerTime);
	}
	return 0.f;
}

float UGameAnimInstance::PlayAnimationDirectly(UAnimSequence* AnimSequence, const FName SlotNodeName,
	const FMontageBlendSettings& BlendInSettings, const FMontageBlendSettings& BlendOutSettings,
	const float InPlayRate, const int32 LoopCount, const float InBlendOutTriggerTime)
{
	if (!AnimSequence || !AnimSequence->GetSkeleton()) return 0.f;

	// create asset using the information
	UAnimMontage* NewMontage = UAnimMontage::CreateSlotAnimationAsDynamicMontage_WithBlendSettings(
		AnimSequence, SlotNodeName, BlendInSettings, BlendOutSettings,
		InPlayRate, LoopCount, InBlendOutTriggerTime);

	if (NewMontage)
	{
		return PlayAnimMontage(NewMontage, InPlayRate);
	}

	return 0.f;
}
