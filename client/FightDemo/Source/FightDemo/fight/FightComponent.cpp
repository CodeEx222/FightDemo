// 


#include "FightComponent.h"

#include "AnimDefine.h"
#include "GameAnimInstance.h"
#include "FightDemo/GameFightCharacter.h"
#include "Kismet/GameplayStatics.h"


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
	FightTimeLineObj = new TFightTimeLine<USkillActionInfo>();

	SkillDataTable = Cast<UDataTable>(LoadObject<UObject>(nullptr,
		TEXT("/Game/Game/data/AttackTable.AttackTable")));
	if (SkillDataTable) {
		// 循环表
		TArray<FName> RowNames = SkillDataTable->GetRowNames();
		for (FName RowName : RowNames) {
			// 获取行数据
			FAttackAnimTable* SkillActionInfo = SkillDataTable->FindRow<FAttackAnimTable>(RowName, TEXT(""));
			if (SkillActionInfo) {
				AttackAnimTableArray.Add(SkillActionInfo);
			}
		}
	}

	const auto OwnCharacterObj = Cast<AGameFightCharacter>(GetOwner());
	if (OwnCharacterObj == nullptr)
	{
		return;
	}
	const auto AnimInstance = Cast<UGameAnimInstance>(OwnCharacterObj->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		return;
	}


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
	case EInputEnum::Defend:
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
	}

}


void UFightComponent::CheckActionTimeLine(int64 CurrentTime, float DeltaTime) const
{
	// 获取当前角色
	//auto OwnCharacterObj = Cast<AGameFightCharacter>(GetOwner());

	if (FightTimeLineObj->GetSize() <= 0)
	{
		return;
	}


	// 动作Line 逻辑处理
	auto CheckActionPtr = FightTimeLineObj->GetFront();
	while (CheckActionPtr != nullptr && CheckActionPtr->ActionTime <= CurrentTime)
	{

		switch (CheckActionPtr->ActionLineState)
		{
		case EActionLineState::SkillStart:
			{
				//检测状态后播放技能
			}
			break;
		default:
			checkNoEntry();
			break;
		}

		// 循环检测下一个node
		if (CheckActionPtr != nullptr)
		{
			FightTimeLineObj->PopById(CheckActionPtr->NodeId);
		}
		CheckActionPtr = FightTimeLineObj->GetFront();
	}

}


void UFightComponent::PlaySkill(FAttackAnimTable* SkillToPlay)
{
	const auto OwnCharacterObj = Cast<AGameFightCharacter>(GetOwner());
	if (OwnCharacterObj == nullptr || SkillToPlay == nullptr)
	{
		return;
	}
	const auto AnimInstance = Cast<UGameAnimInstance>(OwnCharacterObj->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		return;
	}

	// 要播放的动画
	const auto anim = SkillToPlay->ActionAnimMontage;
	const auto PlayMontage = anim.LoadSynchronous();

	AnimInstance->PlayFightMontage(PlayMontage,1,0,FName("Start"), true);

	// 循环动画的所有通知, 注册到时间线当中
	// for (int i = 0; i < PlayMontage->Notifies.Num(); i++)
	// {
	// 	auto notify = PlayMontage->Notifies[i];
	// 	if (notify.Notify != nullptr)
	// 	{
	// 		auto notifyState = Cast<UFightAnimNotify>(notify.Notify);
	// 		if (notifyState != nullptr)
	// 		{
	// 			auto notifyTime = notify.GetTime();
	// 			auto notifyEnum = notifyState->AnimEnum;
	//
	// 			auto skillActionInfo = FightTimeLineObj->GetFreeObj(true);
	// 			skillActionInfo->NodeId = NodeIdGenFactory::GenId();
	// 			skillActionInfo->ActionTime = notifyTime;
	// 			skillActionInfo->ActionLineState = EActionLineState::SkillStart;
	// 			FightTimeLineObj->Add(skillActionInfo);
	// 		}
	// 	}
	// 	else if (notify.NotifyStateClass != nullptr)
	// 	{
	// 		auto notifyState = Cast<UFightAnimNotifyState>(notify.NotifyStateClass);
	// 		if (notifyState != nullptr)
	// 		{
	// 			auto notifyTime = notify.GetTime();
	// 			auto notifyEnum = notifyState->AnimEnum;
	//
	// 			auto skillActionInfo = FightTimeLineObj->GetFreeObj(true);
	// 			skillActionInfo->NodeId = NodeIdGenFactory::GenId();
	// 			skillActionInfo->ActionTime = notifyTime;
	// 			skillActionInfo->ActionLineState = EActionLineState::SkillStart;
	// 			FightTimeLineObj->Add(skillActionInfo);
	// 		}
	// 	}
	//
	// }

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
	for (int i = 0; i < AttackAnimTableArray.Num(); i++)
	{
		auto AnimTable = AttackAnimTableArray[i];
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