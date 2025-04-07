#include "AnimDefine.h"

#include "FightComponent.h"
#include "FightDemo/GameFightCharacter.h"


#if WITH_EDITOR

void UFightAnimNotify::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	CalculateValues();
	Super::PostEditChangeProperty(PropertyChangedEvent);
	// 获取动画序列并刷新
	if (UAnimMontage* AnimSequence = Cast<UAnimMontage>(GetOuter()))
	{
		AnimSequence->RefreshCacheData();
	}
}

void UFightAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation,EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		// 检查是否实现了接口
		const auto Character = Cast<AGameFightCharacter>(MeshComp->GetOwner());
		if (Character != nullptr && Character->FightComponent != nullptr)
		{
			Character->FightComponent->OnAnimNotify(this);
		}
	}
}


void UFightAnimNotifyState::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	CalculateValues();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UFightAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		// 检查是否实现了接口
		const auto Character = Cast<AGameFightCharacter>(MeshComp->GetOwner());
		if (Character != nullptr && Character->FightComponent != nullptr)
		{
			Character->FightComponent->OnAnimNotifyState(this, true);
		}
	}
}

void UFightAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		// 检查是否实现了接口
		const auto Character = Cast<AGameFightCharacter>(MeshComp->GetOwner());
		if (Character != nullptr && Character->FightComponent != nullptr)
		{
			Character->FightComponent->OnAnimNotifyState(this, true);
		}
	}
}
#endif

FString UFightAnimNotify::GetNotifyName()
{
	if (Notify_Name.Len() > 0)
	{
		return Notify_Name;
	}
	return Super::GetNotifyName_Implementation();
}

void UFightAnimNotify::PostInitProperties()
{
	Super::PostInitProperties();
	CalculateValues();
}

void UFightAnimNotifyState::PostInitProperties()
{
	Super::PostInitProperties();
	CalculateValues();
}


void UFightAnimNotify::CalculateValues()
{
	Notify_Name = UEnum::GetDisplayValueAsText(AnimEnum).ToString();
}

FString UFightAnimNotifyState::GetNotifyName_Implementation() const
{
	if (Notify_Name.Len() > 0)
	{

		return Notify_Name;
	}
	return Super::GetNotifyName_Implementation();
}


void UFightAnimNotifyState::CalculateValues()
{
	Notify_Name = UEnum::GetDisplayValueAsText(AnimEnum).ToString();
}

