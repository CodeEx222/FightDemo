// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FightInstance.generated.h"


UENUM(BlueprintType)
enum class ECharaterState : uint8
{
	CharaterState_None UMETA(DisplayName="无状态")  ,
	CharaterState_Attacking UMETA(DisplayName="攻击"),
	CharaterState_AttackingNext UMETA(DisplayName="可以继续攻击"),
	CharaterState_Defending UMETA(DisplayName="防御"),
	CharaterState_Attacked UMETA(DisplayName="受击"),
};

UENUM(BlueprintType)
enum class EAnimNotifyState : uint8
{
	Attack UMETA(DisplayName="攻击")  ,
	AttackNext UMETA(DisplayName="下次攻击"),
	ComboneEnd UMETA(DisplayName="连击结束"),
	InputStart UMETA(DisplayName="预输入开始"),
	InputEnd UMETA(DisplayName="预输入结束"),
	BeAttackedStart UMETA(DisplayName="受击开始"),
	BeAttackedEnd UMETA(DisplayName="受击结束"),
	FanjiAttackedStart UMETA(DisplayName="反击开始"),
	FanjiAttackedEnd UMETA(DisplayName="反击结束"),
};



UENUM(BlueprintType)
enum class EInputEnum : uint8
{
	None = 0 UMETA(DisplayName="无")  ,
	Attack UMETA(DisplayName="攻击")  ,
};

/**
 * 
 */
UCLASS()
class FIGHTDEMO_API UFightInstance : public UGameInstance
{
	GENERATED_BODY()
};