//


#include "FightComponent.h"

#include "FightDemo/Anim/AnimDefine.h"
#include "GameAnimInstance.h"
#include "GameplayTagsManager.h"
#include "PlayerAttributeComponent.h"
#include "ProcessInputComponent.h"
#include "FightDemo/mode/GameFightCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "util/GameTagDefine.h"


AGameFightBase* UFightComponent::GetOwnCharacter()
{
	if (OwnCharacterPtr == nullptr)
	{
		OwnCharacterPtr = Cast<AGameFightBase>(GetOwner());
	}

	check(OwnCharacterPtr != nullptr);

	return OwnCharacterPtr;
}

UGameAnimInstance* UFightComponent::GetAnimInstance()
{
	if (OwnAnimInstance == nullptr)
	{
		const auto Mesh = GetOwnCharacter()->GetMesh();
		UAnimInstance * AnimInstance = Mesh? Mesh->GetAnimInstance() : nullptr;
		check(AnimInstance != nullptr);
		OwnAnimInstance = Cast<UGameAnimInstance>(AnimInstance);
	}

	check(OwnAnimInstance != nullptr);

	return OwnAnimInstance;
}


// Sets default values for this component's properties
UFightComponent::UFightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}





// Called when the game starts
void UFightComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//FightTimeLineObj = new TFightTimeLine<USkillActionInfo>();

	ActiveGameplayTags.AddTag(TAG("game.State.Movement"));
	ActiveMutexGameplayTags = TAG("game.MutexState.Normal");

}


// Called every frame
void UFightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const double GameTime = UGameplayStatics::GetTimeSeconds(this);
	// 移动输入只做输入查询 超过0.5秒的输入清除
	for (int i = 0; i < MoveInputArray.Num(); i++)
	{
		if (GameTime - MoveInputArray[i].GameTime > 0.5)
		{
			MoveInputArray.RemoveAt(i);
		}
	}
}

void UFightComponent::AddInput(EInputEnum InputEnum)
{
	const double GameTime = UGameplayStatics::GetTimeSeconds(this);

	switch (InputEnum)
	{
	case EInputEnum::None:
		return; // 无效输入
	case EInputEnum::MoveForward:
		case EInputEnum::MoveBackward:
		case EInputEnum::MoveLeft:
		case EInputEnum::MoveRight:
		{
			// 移动输入只做记录
			MoveInputArray.Add( {InputEnum, GameTime, true} );

		break;
		}
	case EInputEnum::NormalAttack:
	case EInputEnum::HeavyAttack:
		{
			// 判断当前人物状态是否可以接受指令 普通状态 或者是播放动作可以输入状态
			if (ActiveMutexGameplayTags == TAG("game.MutexState.Normal") ||
				ActiveGameplayTags.HasTagExact(TAG("game.animNotifyState.input" )))
			{
				AddNewInput( {InputEnum, GameTime, true} );
				CheckAttack();
			}
			break;
		}
	case EInputEnum::Defend:
		{
			// 防御输入 打印日志
			//UE_LOG(LogTemp, Warning, TEXT("Defend Input Received"));

			// 判断当前人物状态是否可以接受指令 普通状态 或者是播放动作可以输入状态
			if (ActiveMutexGameplayTags == TAG("game.MutexState.Normal") ||
				ActiveGameplayTags.HasTagExact(TAG("game.animNotifyState.input" )))
			{
				AddNewInput( {InputEnum, GameTime, true} );

				PlayBlock(true);
			}
			{
				// 假如对面是进攻状态,看是否能进入防御反击
				// bool isPlayAttack = false;
				// bool outIsMove;
				// auto target = GetAttackCharacter(outIsMove);
				// if (target != nullptr)
				// {
				// 	// 判断目标对象是否
				// 	if (auto fightComponent = target->GetComponentByClass<UFightComponent>(); fightComponent != nullptr)
				// 	{
				// 		if (fightComponent->GetPlayerActionState(EPlayerState::BeBlockAttack))
				// 		{
				// 			isPlayAttack = true;
				// 			// 播放反击动
				//
				// 		}
				// 	}
				// }

				// if (!isPlayAttack)
				// {
				// 	// 格挡
				// }

			}
		}
		break;
	case EInputEnum::DefendRelease:
		{
			if (ActiveMutexGameplayTags == TAG("game.MutexState.Defending") )
			{
				PlayBlock(false);
			}
		}
		break;
	case EInputEnum::Doge:
		{
			PlayDoge();
		}
		break;


	default: ;
	}
}


void UFightComponent::PlayAttackSkill(FAttackAnimTable* SkillToPlay)
{
	CurrentAnimTable = SkillToPlay;
	// 要播放的动画
	// 需要查看目标或者前方180米是否有人, 有人的话就播放inplace动画 有人比较远播放 move动画 没人就播放inplace动画
	bool bOutIsMove;
	auto target = GetAttackCharacter(bOutIsMove);


	if (target != nullptr)
	{
		// actor 旋转过去
		const auto OwnCharacter = GetOwnCharacter();
		const auto TargetLocation = target->GetActorLocation();
		const auto OwnLocation = OwnCharacter->GetActorLocation();
		const auto TargetRotator = (TargetLocation - OwnLocation).Rotation();
		OwnCharacter->SetGameActorRotation(TargetRotator);
	}

	UAnimMontage* PlayMontage;
	if (bOutIsMove)
	{
		const auto Anim = SkillToPlay->ActionAnimMontageMove;
		PlayMontage = Anim.LoadSynchronous();
	}
	else
	{
		const auto Anim = SkillToPlay->ActionAnimMontageInPlace;
		PlayMontage = Anim.LoadSynchronous();
	}


	CharacterPlayMontage(PlayMontage);
	// 设置攻击状态
	ActiveMutexGameplayTags = TAG("game.MutexState.Attacking");
}

void UFightComponent::PlayBeAttackSkill(AGameFightBase* AttackActor ,FGameplayTag AttackTag)
{
	// 要播放的动画

	// 获取GameInstance

	const auto AttackDir = UFightInstance::CalculateHitDirection(GetOwnCharacter(),AttackActor );
	PlayHit(AttackDir,AttackTag);
	// 设置受击状态
	ActiveMutexGameplayTags = TAG("game.MutexState.Hit");
	// 清空连击状态
	ActiveGameplayTags.RemoveTag(TAG("game.State.Combat.Attack.Combo"));
}

void UFightComponent::PlayBlockBeAttack(AGameFightBase* AttackActor ,FGameplayTag AttackTag)
{
	// // 要播放的动画
	const auto AttackDir = UFightInstance::CalculateHitDirection(GetOwnCharacter(),AttackActor );
	PlayBlockHit(AttackDir,AttackTag);
	// const auto Anim = BlockBeAttackAnimMontage;
	// const auto PlayMontage = Anim.LoadSynchronous();
	//
	// CharacterPlayMontage(PlayMontage);
	// // 设置格挡状态
	//
	// GameCharaterState = ECharaterState::CharaterState_BeAttack;
}

void UFightComponent::PlayBlockBreak(AGameFightBase* AttackActor, FGameplayTag AttackTag)
{
	auto bIsMove = AttackTag.MatchesTag(TAG("game.animNotify.hit.up.heavy")) ||
		AttackTag.MatchesTag(TAG("game.animNotify.hit.down.heavy"));

	FString finallyPath= "/Game/common/FightAnimations/Block/A_Block_Break.A_Block_Break";
	if (bIsMove)
	{
		finallyPath= "/Game/common/FightAnimations/Block/A_Block_Break_M.A_Block_Break_M";
	}

	GetAnimInstance()->PlayAnimSequenceByPath(finallyPath,"SkillSlot");
}


void UFightComponent::PlayDoge()
{
	const auto AnimInstance = GetAnimInstance();

	// bool bIsPlaying = AnimInstance->Montage_IsActive(nullptr);
	//
	// if (bIsPlaying && GetPlayerActionState(EPlayerState::CanAttack))
	// {
	// 	return;
	// }
	//
	// // 要播放的动画
	// const auto Anim = DogeAnimMontage;
	// const auto PlayMontage = Anim.LoadSynchronous();
	//
	// CharacterPlayMontage(PlayMontage);
	// 设置闪避状态
	ActiveMutexGameplayTags = TAG("game.MutexState.Dodging");
	// 清空连击状态
	ActiveGameplayTags.RemoveTag(TAG("game.State.Combat.Attack.Combo"));
}

void UFightComponent::PlayBlock(bool EnterValue)
{
	// 播放防御动作
	const auto AnimInstance = GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->IsInBlock = EnterValue;
	}

	if (EnterValue)
	{
		ActiveMutexGameplayTags = TAG("game.MutexState.Defending");
		// 清空连击状态
		ActiveGameplayTags.RemoveTag(TAG("game.State.Combat.Attack.Combo"));
	}
	else
	{
		ActiveMutexGameplayTags = TAG("game.MutexState.Normal");
	}

}



void UFightComponent::CheckAttack()
{
	const auto SkillToPlay = CheckInput();
	if (SkillToPlay == nullptr)
	{
		return;
	}

	// 要播放新的动作了, 把输入列表的所有新输入检测标识false
	for (int i = 0; i < InputArray.Num(); i++)
	{
		InputArray[i].IsNewCheck = false;
	}

	// 检测当前是否在播放战斗动作
	if (ActiveMutexGameplayTags == TAG("game.MutexState.Attacking"))
	{
		// 如果在播放战斗动作, 进入连击状态
		ActiveGameplayTags.AddTag(TAG("game.State.Combat.Attack.Combo"));
	}

	PlayAttackSkill(SkillToPlay);
}

void UFightComponent::OnAnimNotify(UAnimNotify * Notify)
{
	// 转换为 UFightAnimNotify
	auto fightNotify = Cast<UFightAnimNotify>(Notify);
	if (fightNotify == nullptr)
	{
		return;
	}

	if (fightNotify->AnimTag.MatchesTag(TAG("game.animNotify.hit")))
	{
		// 触发攻击
		bool outIsMove;
		auto target = GetAttackCharacter(outIsMove);
		if (target != nullptr && !outIsMove)
		{
			// 判断目标对象是否
			if (const auto TargetFightComponent = target->GetComponentByClass<UFightComponent>();
				TargetFightComponent != nullptr)
			{
				// 检测是不是无敌
				if (!TargetFightComponent->ActiveGameplayTags.HasTag(TAG("game.SpecialState.Invincible")))
				{
					// 假如目标处于格挡状态, 播放格挡受击动作
					if (TargetFightComponent->ActiveMutexGameplayTags == TAG("game.MutexState.Defending"))
					{
						// 计算伤害
						const auto ChangeValue = CurrentAnimTable->AttackBlockValue;
						if (target->PlayerAttributeComponent != nullptr)
						{
							// 减少目标的血量
							target->PlayerAttributeComponent->ChangeBlockValue(ChangeValue);
						}

						if (target->PlayerAttributeComponent->BlockValue.Value >=
							target->PlayerAttributeComponent->BlockValue.MaxValue )
						{
							// 格挡破防
							TargetFightComponent->PlayBlockBreak(GetOwnCharacter(),fightNotify->AnimTag);
						}
						else
						{
							// 防御受击
							TargetFightComponent->PlayBlockBeAttack(GetOwnCharacter(),fightNotify->AnimTag);
						}


					}
					else
					{
						// 计算伤害
						const auto ChangeValue = CurrentAnimTable->AttackHPValue;
						if (target->PlayerAttributeComponent != nullptr)
						{
							// 减少目标的血量
							target->PlayerAttributeComponent->ChangeHpValue(-ChangeValue);
						}

						if (target->PlayerAttributeComponent->HPValue.Value > 0 )
						{
							// 播放受击动作
							TargetFightComponent->PlayBeAttackSkill(GetOwnCharacter(),fightNotify->AnimTag);
						}
						else
						{
							// 目标死亡

						}


					}
				}
			}
		}
	}
	else if (fightNotify->AnimTag.MatchesTag(TAG("game.animNotify.nextAttack")))
	{
		// 动作结束
		CheckAttack();
	}
}

void UFightComponent::OnAnimNotifyState(UAnimNotifyState * NotifyState, bool bStart)
{
	// 转换为 UFightAnimNotifyState
	auto fightNotifyState = Cast<UFightAnimNotifyState>(NotifyState);
	if (fightNotifyState == nullptr)
	{
		return;
	}

	// 输入区间
	if (bStart)
	{
		ActiveGameplayTags.AddTag(fightNotifyState->AnimTag);
	}
	else
	{
		ActiveGameplayTags.RemoveTag(fightNotifyState->AnimTag);
	}

	if (fightNotifyState->AnimTag.MatchesTag(TAG("game.animNotifyState.lianji")))
	{
		// 连击区间
		if (bStart)
		{
			// 连击开始,检测输入 看看是否可以播放下次动作
			CheckAttack();
		}
		else
		{
			// 假如处于连击当中, 不会清空输入
			if (ActiveGameplayTags.HasTag(TAG("game.State.Combat.Attack.Combo")))
			{

			}
			else
			{
				// 连击结束,清空输入
				InputArray.Empty();
			}
		}
	}

}

#pragma region 玩家输入检测

void  UFightComponent::AddNewInput(const FInputElement& NewInput)
{
	// 拿取 InputArray 最后一个元素
	if (InputArray.Num() > 0)
	{
		if (const auto LastInput = InputArray.Last(); LastInput.IsNewCheck)
		{
			// 如果最后一个元素是新的输入,那么就覆盖
			InputArray.RemoveAt(InputArray.Num() - 1);
		}
	}
	InputArray.Add(NewInput);
}

FAttackAnimTable* UFightComponent::CheckInput()
{
	//const double GameTime = UGameplayStatics::GetTimeSeconds(this);

	// 检测 InputArray 最后一个是否为新的输入,否则不检测
	if (InputArray.Num() == 0 || !InputArray[InputArray.Num()-1].IsNewCheck )
	{
		return nullptr;
	}

	// 循环所有的出招表,看是否有符合出招的招式
	// 获取GameInstance
	const auto GameInstance = Cast<UFightInstance>(GetWorld()->GetGameInstance());

	for (int i = 0; i < GameInstance->AttackAnimTableArray.Num(); i++)
	{
		const auto AnimTable = GameInstance->AttackAnimTableArray[i];
		bool bIsfind = false;

		// if (AnimTable->MoveInputs.Num() > 0 && MoveInputArray.Num() > 0)
		// {
		// 	for (int j = 0; j < AnimTable->MoveInputs.Num(); j++)
		// 	{
		// 		auto checkEnum = AnimTable->MoveInputs[j];
		// 		TArray<EInputEnum> checkArray;
		// 		for (auto Element : MoveInputArray)
		// 		{
		// 			checkArray.Add(Element.InputEnum);
		// 		}
		//
		// 		if (!checkArray.Contains(checkEnum))
		// 		{
		// 			bIsfind = false;
		// 			break;
		// 		}
		// 	}
		// }

		// 检测 input是否一致
		if (AnimTable->Inputs.Num() == InputArray.Num())
		{
			bIsfind = true;
			// 完全匹配
			for (int j = 0; j < AnimTable->Inputs.Num(); ++j)
			{
				if (AnimTable->Inputs[j] != InputArray[j].InputEnum)
				{
					bIsfind = false;
					break;
				}
			}
		}


		if (bIsfind)
		{
			// 找到匹配的动作
			return AnimTable;
		}

	}

	return nullptr;
}

#pragma endregion


AGameFightBase* UFightComponent::GetAttackCharacter(bool& OutIsMove)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameFightBase::StaticClass(), FoundActors);

	OutIsMove = false;
	for (auto Actor : FoundActors)
	{
		// 假如角色
		auto Target = Cast<AGameFightBase>(Actor);
		if (Target == nullptr)
		{
			continue;
		}

		if (Target == GetOwner())
		{
			continue;
		}

		// 判断位置在1.8米以内, 并且方向在正前方30度
		if (FVector::Distance(GetOwner()->GetActorLocation(), Target->GetActorLocation()) < 160)
		{
			const auto ForwardVector = GetOwner()->GetActorForwardVector();
			const auto TargetVector = Target->GetActorLocation() - GetOwner()->GetActorLocation();
			const auto Angle = FMath::Acos(FVector::DotProduct(ForwardVector, TargetVector.GetSafeNormal()));
			if (FMath::RadiansToDegrees(Angle) < 30)
			{
				OutIsMove = false;
				return Target;
			}
		}

		if (FVector::Distance(GetOwner()->GetActorLocation(), Target->GetActorLocation()) < 300)
		{
			const auto ForwardVector = GetOwner()->GetActorForwardVector();
			const auto TargetVector = Target->GetActorLocation() - GetOwner()->GetActorLocation();
			const auto Angle = FMath::Acos(FVector::DotProduct(ForwardVector, TargetVector.GetSafeNormal()));
			if (FMath::RadiansToDegrees(Angle) < 30)
			{
				OutIsMove = true;
				return Target;
			}
		}
	}

	return nullptr;
}


#pragma region 动画结束通知

void UFightComponent::CharacterPlayMontage(UAnimMontage* Montage, float InPlayRate, const FName StartSectionName,
	const EMontagePlayReturnType ReturnValueType)
{
	AnimPlayInstanceID++;
	FOnMontageBlendingOutStarted BlendingOutDelegate;
	BlendingOutDelegate.BindUObject(this, &UFightComponent::OnMontagePlayBlendingOut,AnimPlayInstanceID);

	GetAnimInstance()->PlayAnimMontage(Montage,1,StartSectionName,ReturnValueType);
	GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendingOutDelegate, Montage);
}

void UFightComponent::OnMontagePlayBlendingOut(UAnimMontage* Montage, bool bInterrupted, int32 InstanceID)
{
	// UE_LOG( LogTemp, Warning, TEXT("OnMontagePlayBlendingOut: %s, Interrupted: %s, Id: %d"),
	// 		*Montage->GetName(), bInterrupted ? TEXT("true") : TEXT("false") ,InstanceID);

	// 动作融出, 这时候出现几种情况
	// 1. 动作播放完毕, 后续没有其他动作了, 这时候要把互斥状态置位普通, 连击状态清空
	// 2. 动作播放完毕, 后续攻击动作
	// 3. 动作被打断,

	// 说明后续没有动作了
	if (InstanceID == AnimPlayInstanceID)
	{
		ActiveMutexGameplayTags = TAG("game.MutexState.Normal");
		ActiveGameplayTags.RemoveTag(TAG("game.State.Combat.Attack.Combo"));
	}
}

#pragma endregion


void UFightComponent::PlayHit(EHitDirection8 AttackerDir, FGameplayTag AttackTag)
{
	FString AnimPath= "/Game/common/FightAnimations/Hit/";


	FString SkillName = "";
	if (AttackTag.MatchesTag(TAG("game.animNotify.hit.up")))
	{
		SkillName = "A_HitHead_";
	}
	else if (AttackTag.MatchesTag(TAG("game.animNotify.hit.down")))
	{
		SkillName = "A_HitLeg_";
	}

	auto bIsMove = AttackTag.MatchesTag(TAG("game.animNotify.hit.up.heavy")) ||
		AttackTag.MatchesTag(TAG("game.animNotify.hit.down.heavy"));

	if (bIsMove)
	{
		AnimPath += "Move/";
	}
	else
	{
		AnimPath += "InPlace/";
	}


	switch (AttackerDir)
	{
	case EHitDirection8::None:
	case EHitDirection8::Front:
		SkillName += "F_";
		break;
	case EHitDirection8::FrontRight:
		SkillName += "F_";
		break;
	case EHitDirection8::Right:
		SkillName += bIsMove ? "Side_R_": "R_";
		break;
	case EHitDirection8::BackRight:
		SkillName = "A_Hit_Back_";
		break;
	case EHitDirection8::Back:
		SkillName = "A_Hit_Back_";
		break;
	case EHitDirection8::BackLeft:
		SkillName = "A_Hit_Back_";
		break;
	case EHitDirection8::Left:
		SkillName += bIsMove ? "Side_L_" : "L_";
		break;
	case EHitDirection8::FrontLeft:
		SkillName += "F_";
		break;
	}

	if (bIsMove)
	{
		SkillName += "M";
	}
	else
	{
		SkillName += "IP";
	}

	auto finallyPath = AnimPath + SkillName + "." + SkillName;

	// /Game/common/FightAnimations/Hit/InPlace/A_Hit_Back_IP.A_Hit_Back_IP
	// /Game/common/FightAnimations/Hit/Move/A_HiBack_M.A_HiBack_M
	// 打印到屏幕上
	UE_LOG(LogTemp, Warning, TEXT("PlayHit AnimPath: %s"), *finallyPath);

	GetAnimInstance()->PlayAnimSequenceByPath(finallyPath,"SkillSlot");
}

void UFightComponent::PlayBlockHit(EHitDirection8 AttackerDir, FGameplayTag AttackTag)
{

	// /Game/common/FightAnimations/Hit/InPlace/A_Hit_Back_IP.A_Hit_Back_IP
	// /Game/common/FightAnimations/Hit/Move/A_HiBack_M.A_HiBack_M

	FString AnimPath= "/Game/common/FightAnimations/Block/";

	FString SkillName = "";
	if (AttackTag.MatchesTag(TAG("game.animNotify.hit.down")))
	{
		SkillName = "A_Block_Leg";
	}
	else
	{
		SkillName = "A_BlockHit_";

		auto bIsMove = AttackTag.MatchesTag(TAG("game.animNotify.hit.up.heavy")) ||
		AttackTag.MatchesTag(TAG("game.animNotify.hit.down.heavy"));

		if (bIsMove)
		{
			AnimPath += "Move/";
		}
		else
		{
			AnimPath += "InPlace/";
		}

		switch (AttackerDir)
		{
		case EHitDirection8::None:
		case EHitDirection8::Front:
			SkillName += "F_";
			break;
		case EHitDirection8::FrontRight:
			SkillName += "F_";
			break;
		case EHitDirection8::Right:
			SkillName += "R_";
			break;
		case EHitDirection8::BackRight:
			SkillName = "R_";
			break;
		case EHitDirection8::Back:
			SkillName = "";
			break;
		case EHitDirection8::BackLeft:
			SkillName = "L_";
			break;
		case EHitDirection8::Left:
			SkillName += "L_";
			break;
		case EHitDirection8::FrontLeft:
			SkillName += "F_";
			break;
		}

		if (bIsMove)
		{
			SkillName += "M";
		}
		else
		{
			SkillName += "IP";
		}
	}


	auto finallyPath = AnimPath + SkillName + "." + SkillName;

	// /Game/common/FightAnimations/Hit/InPlace/A_Hit_Back_IP.A_Hit_Back_IP
	// /Game/common/FightAnimations/Hit/Move/A_HiBack_M.A_HiBack_M
	// 打印到屏幕上
	UE_LOG(LogTemp, Warning, TEXT("PlayBlockHit AnimPath: %s"), *finallyPath);

	GetAnimInstance()->PlayAnimSequenceByPath(finallyPath,"SkillSlot");
}





