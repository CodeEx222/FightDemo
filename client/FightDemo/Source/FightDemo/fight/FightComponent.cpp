// 


#include "FightComponent.h"

#include "AnimDefine.h"
#include "GameAnimInstance.h"
#include "FightDemo/GameFightCharacter.h"
#include "Kismet/GameplayStatics.h"


AGameFightCharacter* UFightComponent::GetOwnCharacter()
{
	if (OwnCharacterPtr == nullptr)
	{
		OwnCharacterPtr = Cast<AGameFightCharacter>(GetOwner());
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

float UFightComponent::PlayAnimMontage(class UAnimMontage* AnimMontage,
                                       const float InPlayRate, const FName StartSectionName, const EMontagePlayReturnType ReturnValueType)
{
	if(UAnimInstance * AnimInstance = GetAnimInstance(); AnimMontage && AnimInstance )
	{
		if (float const Duration = AnimInstance->Montage_Play(AnimMontage, InPlayRate,ReturnValueType); Duration > 0.f)
		{
			// Start at a given Section.
			if( StartSectionName != NAME_None )
			{
				AnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
			}

			return Duration;
		}
	}

	return 0.f;
}


// Sets default values for this component's properties
UFightComponent::UFightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PlayerActionStateBitset = std::make_shared<std::bitset<32>>();

}





// Called when the game starts
void UFightComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//FightTimeLineObj = new TFightTimeLine<USkillActionInfo>();

	const auto AnimInstance = GetAnimInstance();

	AnimInstance->OnMontageEnded.AddDynamic( this, &UFightComponent::OnMontageEnded);
	AnimInstance->OnMontageBlendingOut.AddDynamic( this, &UFightComponent::OnMontageBlendingOut);



	// 可以攻击
	SetPlayerActionState(EPlayerState::CanAttack);
	// 可以记录输入
	SetPlayerActionState(EPlayerState::CanRecordInput);
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

void UFightComponent::AddInput(EInputEnum inputEnum)
{
	const double GameTime = UGameplayStatics::GetTimeSeconds(this);

	switch (inputEnum)
	{
	case EInputEnum::None:
		break;
	case EInputEnum::MoveForward:
	case EInputEnum::MoveBackward:
	case EInputEnum::MoveLeft:
	case EInputEnum::MoveRight:
		// 移动输入只做记录
		MoveInputArray.Add( {inputEnum, GameTime, true} );
		break;
	case EInputEnum::NormalAttack:
	case EInputEnum::HeavyAttack:
		// 判断当前人物状态是否可以接受指令
		if ( GetPlayerActionState(EPlayerState::CanRecordInput))
		{
			AddNewInput( {inputEnum, GameTime, true} );
			// 假设当前人物没有在攻击状态的话,就可以检测出招
			if ( GetPlayerActionState(EPlayerState::CanAttack))
			{
				CheckAttack();
			}

		}

		break;
	case EInputEnum::Defend:
		// 防御输入 打印日志
		//UE_LOG(LogTemp, Warning, TEXT("Defend Input Received"));
		if (GameCharaterState == ECharaterState::CharaterState_None)
		{
			// 假如对面是进攻状态,看是否能进入防御反击
			bool isPlayAttack = false;
			auto target = GetAttackCharacter();
			if (target != nullptr)
			{
				// 判断目标对象是否
				if (auto fightComponent = target->GetComponentByClass<UFightComponent>(); fightComponent != nullptr)
				{
					if (fightComponent->GetPlayerActionState(EPlayerState::BeBlockAttack))
					{
						isPlayAttack = true;
						// 播放反击动
						fightComponent->PlayBeAttackSkill(CurrentAnimTable);
						PlayBlockAttackSkill(fightComponent->CurrentAnimTable);
					}
				}
			}

			if (!isPlayAttack)
			{
				// 格挡
			}

		}
		break;
	case EInputEnum::Doge:
		{
			PlayDoge();
		}

		break;
	}



}


void UFightComponent::PlaySkill(FAttackAnimTable* SkillToPlay)
{
	CurrentAnimTable = SkillToPlay;
	// 要播放的动画
	const auto Anim = SkillToPlay->ActionAnimMontage;
	const auto PlayMontage = Anim.LoadSynchronous();

	PlayAnimMontage(PlayMontage,1);
	// 设置攻击状态
	GameCharaterState = ECharaterState::CharaterState_Attacking;
}

void UFightComponent::PlayBeAttackSkill(FAttackAnimTable* SkillToPlay)
{
	const auto AnimInstance = GetAnimInstance();

	// 要播放的动画
	const auto anim = SkillToPlay->BeAttackAnimMontage;
	// 随机一个动画
	if (anim.Num() == 0)
	{
		return;
	}
	const int32 RandomIndex = FMath::RandRange(0, anim.Num() - 1);
	const auto animMontage = anim[RandomIndex];

	const auto PlayMontage = animMontage.LoadSynchronous();

	AnimInstance->PlayFightMontage(PlayMontage,1,0,FName("Start"), true);


}

void UFightComponent::PlayBlockAttackSkill(FAttackAnimTable* SkillToPlay)
{
	const auto AnimInstance = GetAnimInstance();

	// 要播放的动画
	const auto AnimArray = SkillToPlay->BlockAttackAnimMontage;
	// 随机一个动画
	if (AnimArray.Num() == 0)
	{
		return;
	}
	const int32 RandomIndex = FMath::RandRange(0, AnimArray.Num() - 1);
	const auto AnimMontage = AnimArray[RandomIndex];

	const auto PlayMontage = AnimMontage.LoadSynchronous();

	AnimInstance->PlayFightMontage(PlayMontage,1,0,FName("Start"), true);


}


void UFightComponent::PlayDoge()
{
	const auto AnimInstance = GetAnimInstance();

	bool bIsPlaying = AnimInstance->Montage_IsActive(nullptr);

	if (bIsPlaying && GetPlayerActionState(EPlayerState::CanAttack))
	{
		return;
	}

	// 要播放的动画
	const auto Anim = DogeAnimMontage;
	const auto PlayMontage = Anim.LoadSynchronous();

	AnimInstance->PlayFightMontage(PlayMontage,1,0,FName("Start"), true);
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

	//把接受输入置为false
	SetPlayerActionState(EPlayerState::CanRecordInput,false);

	// 检测当前是否在播放战斗动作
	if (GetPlayerActionState(EPlayerState::InPlayAttack))
	{
		// 如果在播放战斗动作,那么就把当前的动作结束, 进入连击状态
		SetPlayerActionState(EPlayerState::InComboNext);
	}
	SetPlayerActionState(EPlayerState::InPlayAttack);

	SetPlayerActionState(EPlayerState::CanAttack,false);
	PlaySkill(SkillToPlay);
}

void UFightComponent::OnAnimNotify(UAnimNotify * Notify)
{
	// 转换为 UFightAnimNotify
	auto fightNotify = Cast<UFightAnimNotify>(Notify);
	if (fightNotify == nullptr)
	{
		return;
	}

	switch (fightNotify->AnimEnum)
	{
		case  EFightAnimNotify::Attack:
		{
			// 触发攻击
			auto target = GetAttackCharacter();
			if (target != nullptr)
			{
				// 判断目标对象是否
				if (auto TargetFightComponent = target->GetComponentByClass<UFightComponent>(); TargetFightComponent != nullptr)
				{
					TargetFightComponent->PlayBeAttackSkill(CurrentAnimTable);
					TargetFightComponent->HPNum -= CurrentAnimTable->AttackHPValue;
					target->HpChangeView(TargetFightComponent->HPNum,100);
				}
			}


		}
	case EFightAnimNotify::AttackNext:
		{
			// 可以播放下次攻击
			SetPlayerActionState(EPlayerState::CanAttack);
			CheckAttack();
		}
	default: ;
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

	switch (fightNotifyState->AnimEnum)
	{
	case EFightAnimStateNotify::None:
		break;
	case EFightAnimStateNotify::PlayState:
			SetPlayerActionState(EPlayerState::InPlayAttack,bStart);

			if (bStart)
			{
				// 新的开始, 把连击置空
				SetPlayerActionState(EPlayerState::InComboNext, false);
			}
			else
			{
				// 连击有效区间
				if (GetPlayerActionState(EPlayerState::InComboNext))
				{
					// 是连击结束的,就不需要清空输入了
				}
				else
				{
					// 其他结束,清空输入
					InputArray.Empty();
				}
			}

		break;
	case EFightAnimStateNotify::FanJiState:
		// 反击区间
		SetPlayerActionState(EPlayerState::BeBlockAttack, bStart);
		break;
	case EFightAnimStateNotify::InoputState:
		// 输入区间
		SetPlayerActionState(EPlayerState::CanRecordInput, true);
		break;
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

#pragma region 玩家状态检测

void UFightComponent::SetPlayerActionState(EPlayerState Type, const bool Value /* = true */) const
{
	PlayerActionStateBitset->set(static_cast<size_t>(Type), Value);
}

bool UFightComponent::GetPlayerActionState(EPlayerState Type) const
{
	return  PlayerActionStateBitset->test(static_cast<size_t>(Type));
}

#pragma endregion

AGameFightCharacter* UFightComponent::GetAttackCharacter()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameFightCharacter::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		// 假如角色
		auto Target = Cast<AGameFightCharacter>(Actor);
		if (Target == nullptr)
		{
			continue;
		}

		if (Target == GetOwner())
		{
			continue;
		}

		// 判断位置在1米以内, 并且方向在正前方30度
		if (FVector::Distance(GetOwner()->GetActorLocation(), Target->GetActorLocation()) < 150)
		{
			const auto ForwardVector = GetOwner()->GetActorForwardVector();
			const auto TargetVector = Target->GetActorLocation() - GetOwner()->GetActorLocation();
			const auto Angle = FMath::Acos(FVector::DotProduct(ForwardVector, TargetVector.GetSafeNormal()));
			if (FMath::RadiansToDegrees(Angle) < 30)
			{
				return Target;
			}
		}
	}

	return nullptr;
}


#pragma region 动画结束通知

void UFightComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG( LogTemp, Warning, TEXT("Montage Ended: %s, Interrupted: %d"), *Montage->GetName(), bInterrupted);
	OnMontagePlayerEnd(Montage, bInterrupted);
}

void UFightComponent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG( LogTemp, Warning, TEXT("Montage Blending Out: %s, Interrupted: %d"), *Montage->GetName(), bInterrupted);
	OnMontagePlayerEnd(Montage, bInterrupted);
}

void UFightComponent::OnMontagePlayerEnd(UAnimMontage* Montage, bool bInterrupted)
{

}

#pragma endregion




