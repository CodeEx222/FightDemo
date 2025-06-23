#include "AnimDefine.h"

#include "AnimNotifyProcess.h"
#include "GameplayTagsManager.h"
#include "mode/GameFightCharacter.h"


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
		if (IAnimNotifyProcess* AnimNotifyProcess = Cast<IAnimNotifyProcess>(MeshComp->GetOwner()))
		{
			AnimNotifyProcess->DoAnimNotify(this);
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
		if (IAnimNotifyProcess* AnimNotifyProcess = Cast<IAnimNotifyProcess>(MeshComp->GetOwner()))
		{
			AnimNotifyProcess->DoAnimNotifyState(this, true);
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
		if (IAnimNotifyProcess* AnimNotifyProcess = Cast<IAnimNotifyProcess>(MeshComp->GetOwner()))
		{
			AnimNotifyProcess->DoAnimNotifyState(this, false);
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

	auto allTagsArray = AnimTag.GetGameplayTagParents();

	FString ShowName = "";
	for (FGameplayTag tagElement : allTagsArray)
	{
		if (tagElement.RequestDirectParent() == FGameplayTag::EmptyTag )
		{
			// 去除根标签
			continue;
		}

		FString TagComment;
		FName TagSource;
		bool bTagIsExplicit;
		bool bTagIsRestricted;
		bool bTagAllowsNonRestrictedChildren;

		UGameplayTagsManager::Get().GetTagEditorData(tagElement.GetTagName(),
			TagComment, TagSource,
			bTagIsExplicit,
			bTagIsRestricted, bTagAllowsNonRestrictedChildren);

		if (TagComment.Len() > 0)
		{
			ShowName = TagComment + (!ShowName.IsEmpty() ? TEXT(".") : TEXT("")) + ShowName;
		}
		else
		{
			ShowName  =  tagElement.GetTagName().ToString() + (!ShowName.IsEmpty() ? TEXT(".") : TEXT("")) + ShowName;
		}
	}

	Notify_Name = ShowName;
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
	auto allTagsArray = AnimTag.GetGameplayTagParents();

	FString ShowName = "";
	for (FGameplayTag tagElement : allTagsArray)
	{
		if (tagElement.RequestDirectParent() == FGameplayTag::EmptyTag )
		{
			// 去除根标签
			continue;
		}

		FString TagComment;
		FName TagSource;
		bool bTagIsExplicit;
		bool bTagIsRestricted;
		bool bTagAllowsNonRestrictedChildren;

		UGameplayTagsManager::Get().GetTagEditorData(tagElement.GetTagName(),
			TagComment, TagSource,
			bTagIsExplicit,
			bTagIsRestricted, bTagAllowsNonRestrictedChildren);

		if (TagComment.Len() > 0)
		{
			ShowName = TagComment + (!ShowName.IsEmpty() ? TEXT(".") : TEXT("")) + ShowName;
		}
		else
		{
			ShowName  =  tagElement.GetTagName().ToString() + (!ShowName.IsEmpty() ? TEXT(".") : TEXT("")) + ShowName;
		}
	}

	Notify_Name = ShowName;
}

