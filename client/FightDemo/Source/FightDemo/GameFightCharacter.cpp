


#include "GameFightCharacter.h"

#include "game/FightInstance.h"

// Sets default values
AGameFightCharacter::AGameFightCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameFightCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameFightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGameFightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGameFightCharacter::EnterBlock()
{
	GameCharaterState =  ECharaterState::CharaterState_Defending;
	// 判断这时候目标进入
	if (PlayerFightTarget != nullptr && PlayerFightTarget->canFanJi)
	{
		if (IsAttackPlayer(PlayerFightTarget))
		{
			// 播放反击动画
			AttackNum = 0;
			auto anim  = FightAttackAnimArray[AttackNum];

			if (anim != nullptr)
			{
				FightAnimMontage = anim;
				PlayGameMontage(anim);
				GameCharaterState = ECharaterState::CharaterState_Attacking;
			}
			// 对面播放受击
			PlayerFightTarget->BeAttackPlayer(1);

			ChangeTime();
		}


	}
}

void AGameFightCharacter::ExitBlock()
{
	if (GameCharaterState == ECharaterState::CharaterState_Defending)
	{
		GameCharaterState = ECharaterState::CharaterState_None;
	}

}

void AGameFightCharacter::AttackPlayer()
{

	switch (GameCharaterState)
	{
	case ECharaterState::CharaterState_None:
	case ECharaterState::CharaterState_AttackingNext:
		{
			if (FightAttackAnimArray.Num() <= AttackNum)
			{
				AttackNum = 0;
			}

			// 获取连击索引值
			auto anim  = FightAttackAnimArray[AttackNum];

			if (anim != nullptr)
			{
				FightAnimMontage = anim;
				PlayGameMontage(anim);
				GameCharaterState = ECharaterState::CharaterState_Attacking;
			}
		}
		break;
	case ECharaterState::CharaterState_Attacking:
		if (canInputRecord)
		{
			AllInputs = EInputEnum::Attack;
		}
		break;

	default: ;
	}
}

void AGameFightCharacter::BeAttackPlayer(int mode)
{
	if (mode == 0)
	{
		// 获取连击索引值
		auto anim  = FightBeAttackAnimArray[0];

		if (GameCharaterState == ECharaterState::CharaterState_Defending)
		{
			anim  = FightBeAttackAnimArray[1];
		}


		if (anim != nullptr)
		{
			FightAnimMontage = anim;
			PlayGameMontage(anim);
		}
	}
	else if (mode == 1)
	{
		// 获取连击索引值
		auto anim  = FightBeAttackAnimArray[2];

		if (anim != nullptr)
		{
			FightAnimMontage = anim;
			PlayGameMontage(anim);
		}
	}

}

void AGameFightCharacter::MontagePlayerEnd()
{
	FightAnimMontage = nullptr;
	canInputRecord = false;
	AllInputs = EInputEnum::None;
	canFanJi = false;
}

void AGameFightCharacter::Anim_Notify(EAnimNotifyState notifyState)
{
	switch (notifyState)
	{
	case EAnimNotifyState::Attack:
		{
			auto target = PlayerFightTarget;
			ChangeTarget();
			target = PlayerFightTarget;
			if (target == nullptr)
			{
				return;
			}

			if (IsAttackPlayer(target))
			{
				target->BeAttackPlayer(0);
			}


		}
		break;
	case EAnimNotifyState::AttackNext:
		GameCharaterState = ECharaterState::CharaterState_AttackingNext;
		AttackNum++;
		if (AllInputs == EInputEnum::Attack)
		{
			AttackPlayer();
			AllInputs = EInputEnum::None;
		}
		break;
	case EAnimNotifyState::ComboneEnd:
		GameCharaterState = ECharaterState::CharaterState_None;
		AttackNum = 0;
		break;
	case EAnimNotifyState::InputStart:
		canInputRecord = true;
		break;
	case EAnimNotifyState::InputEnd:
		canInputRecord = false;
		break;
	case EAnimNotifyState::BeAttackedStart:
		GameCharaterState = ECharaterState::CharaterState_Attacked;
		break;
	case EAnimNotifyState::BeAttackedEnd:
		GameCharaterState = ECharaterState::CharaterState_None;
		break;
	case EAnimNotifyState::FanjiAttackedStart:
		canFanJi = true;
		break;
		case EAnimNotifyState::FanjiAttackedEnd:
			canFanJi = false;
		break;
	default:
		break;
	}
}


bool AGameFightCharacter::IsAttackPlayer(AGameFightCharacter* target)
{
	auto selfPos = this->GetActorLocation();
	auto targetPos = target->GetActorLocation();

	auto dir = targetPos - selfPos;


	// 计算距离
	auto len = dir.Length();
	if (len < 200)
	{
		dir.Normalize();
		auto selfVertor = this->GetActorForwardVector();

		float angle = acos(FVector::DotProduct(selfVertor, dir)/ (selfVertor.Size()*selfVertor.Size()) ) * 180 / PI;

		if (angle <= 40)
		{
			return true;
		}

	}

	return false;

}

