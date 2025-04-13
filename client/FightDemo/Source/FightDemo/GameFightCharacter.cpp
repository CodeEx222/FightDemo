


#include "GameFightCharacter.h"

#include "fight/AnimDefine.h"
#include "fight/FightComponent.h"
#include "game/FightInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameFightCharacter::AGameFightCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FightComponent = CreateDefaultSubobject<UFightComponent>(TEXT("FightComponent"));

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

void AGameFightCharacter::UpdateActorRotator()
{
	// 处于移动中 actor朝向向摄影机朝向旋转
	auto camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	auto cameraRot = camera->GetCameraRotation();
	// 获取 cameraRot xy平面上的旋转值
	cameraRot.Pitch = 0;
	cameraRot.Roll = 0;

	auto actorRot = GetActorRotation();
	auto setActorRot = actorRot;
	setActorRot.Pitch = 0;
	setActorRot.Roll = 0;


	auto gameDelta = GetWorld()->GetDeltaSeconds();
	auto rot = FMath::RInterpTo(setActorRot, cameraRot, gameDelta  , 5);
	actorRot.Yaw = rot.Yaw;
	SetActorRotation(actorRot);
}

void AGameFightCharacter::EnterBlock()
{
	FightComponent->GameCharaterState =  ECharaterState::CharaterState_Defending;
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
				FightComponent->GameCharaterState = ECharaterState::CharaterState_Attacking;
			}
			// 对面播放受击
			PlayerFightTarget->BeAttackPlayer(1);

			ChangeTime();
		}


	}
}

void AGameFightCharacter::ExitBlock()
{
	if (FightComponent->GameCharaterState == ECharaterState::CharaterState_Defending)
	{
		FightComponent->GameCharaterState = ECharaterState::CharaterState_None;
	}

}

void AGameFightCharacter::AttackPlayer()
{

	switch (FightComponent->GameCharaterState)
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
				FightComponent->GameCharaterState = ECharaterState::CharaterState_Attacking;
			}
		}
		break;
	case ECharaterState::CharaterState_Attacking:

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

		if (FightComponent->GameCharaterState == ECharaterState::CharaterState_Defending)
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
	canFanJi = false;
}

// void AGameFightCharacter::Anim_Notify(EAnimNotifyState notifyState)
// {
// 	switch (notifyState)
// 	{
// 	case EAnimNotifyState::Attack:
// 		{
// 			auto target = PlayerFightTarget;
// 			ChangeTarget();
// 			target = PlayerFightTarget;
// 			if (target == nullptr)
// 			{
// 				return;
// 			}
//
// 			if (IsAttackPlayer(target))
// 			{
// 				target->BeAttackPlayer(0);
// 			}
//
//
// 		}
// 		break;
// 	case EAnimNotifyState::AttackNext:
// 		FightComponent->GameCharaterState = ECharaterState::CharaterState_AttackingNext;
// 		AttackNum++;
// 		break;
// 	case EAnimNotifyState::ComboneEnd:
// 		FightComponent->GameCharaterState = ECharaterState::CharaterState_None;
// 		AttackNum = 0;
// 		break;
// 	case EAnimNotifyState::InputStart:
// 		FightComponent->canInputRecord = true;
// 		break;
// 	case EAnimNotifyState::InputEnd:
// 		FightComponent->canInputRecord = false;
// 		break;
// 	case EAnimNotifyState::BeAttackedStart:
// 		FightComponent->GameCharaterState = ECharaterState::CharaterState_Attacked;
// 		break;
// 	case EAnimNotifyState::BeAttackedEnd:
// 		FightComponent->GameCharaterState = ECharaterState::CharaterState_None;
// 		break;
// 	case EAnimNotifyState::FanjiAttackedStart:
// 		canFanJi = true;
// 		break;
// 		case EAnimNotifyState::FanjiAttackedEnd:
// 			canFanJi = false;
// 		break;
// 	default:
// 		break;
// 	}
// }
//

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


