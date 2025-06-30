


#include "GameAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"
#include "mode/GameFightCharacter.h"


void UGameAnimInstance::CalMoveDirByVelocity()
{
	auto Velocity = gameCharacter ? gameCharacter->GetVelocity() : FVector::ZeroVector;
	// 1. 检查是否静止
	if (Velocity.IsNearlyZero(0.1f))
	{
		MoveDirection =  EHitDirection8::None;
		return ;
	}

	// 2. 转换速度到Actor局部空间
	FVector LocalVelocity = gameCharacter->GetActorTransform().InverseTransformVectorNoScale(Velocity);
	LocalVelocity.Z = 0; // 忽略垂直运动
	if (LocalVelocity.IsNearlyZero(0.1f))
	{
		MoveDirection =  EHitDirection8::None;
		return ;
	}

	LocalVelocity.Normalize(); // 单位化

	// 3. 计算角度 (0° = 正前, 逆时针增加)
	float AngleRad = FMath::Atan2(LocalVelocity.Y, LocalVelocity.X); // Y=右向, X=前向
	float AngleDeg = FMath::RadiansToDegrees(AngleRad);
	if (AngleDeg < 0) AngleDeg += 360; // 转换为0-360范围

	// 4. 映射到8方向 (22.5° 偏移使扇区居中)
	const float SectorSize = 45.0f;
	int32 Sector = FMath::FloorToInt((AngleDeg + 22.5f) / SectorSize) % 8;

	MoveActionX = 0;
	MoveActionY = 0;

	switch (Sector)
	{
	case 0:
		MoveDirection = EHitDirection8::Front;
		MoveActionX = 0;
		MoveActionY = 1;
		break;     // 0°-45°
	case 1:
		MoveDirection =  EHitDirection8::FrontRight;
		MoveActionX = 1;
		MoveActionY = 1;
		break; // 45°-90°
	case 2:
		MoveDirection =  EHitDirection8::Right;
		MoveActionX = 1;
		MoveActionY = 0;
		break;       // 90°-135°
	case 3:
		MoveDirection =  EHitDirection8::BackRight;
		MoveActionX = 1;
		MoveActionY = -1;
		break;// 135°-180°
	case 4:
		MoveDirection =  EHitDirection8::Back;
		MoveActionX = 0;
		MoveActionY = -1;
		break;    // 180°-225°
	case 5:
		MoveDirection =  EHitDirection8::BackLeft;
		MoveActionX = -1;
		MoveActionY = -1;
		break;// 225°-270°
	case 6:
		MoveDirection =  EHitDirection8::Left;
		MoveActionX = -1;
		MoveActionY = 0;
		break;         // 270°-315°
	case 7:
		MoveDirection =  EHitDirection8::FrontLeft;
		MoveActionX = -1;
		MoveActionY = 1;
		break;  // 315°-360°
	default:
		MoveDirection =  EHitDirection8::None;
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
