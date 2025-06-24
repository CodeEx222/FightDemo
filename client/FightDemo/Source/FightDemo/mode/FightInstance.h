// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/GameInstance.h"
#include "Blueprint/BlueprintSupport.h"
#include "UObject/UnrealNames.h"

#include "FightInstance.generated.h"

// 定义8方向枚举
UENUM(BlueprintType)
enum class EHitDirection8 : uint8
{
	None       UMETA(DisplayName = "无方向"),
	Front      UMETA(DisplayName = "前"),
	FrontRight UMETA(DisplayName = "前右"),
	Right      UMETA(DisplayName = "右"),
	BackRight  UMETA(DisplayName = "后右"),
	Back       UMETA(DisplayName = "后"),
	BackLeft   UMETA(DisplayName = "后左"),
	Left       UMETA(DisplayName = "左"),
	FrontLeft  UMETA(DisplayName = "前左")
};

UENUM(BlueprintType)
enum class ECharaterState : uint8
{
	CharaterState_None UMETA(DisplayName="无状态")  ,
	CharaterState_Attacking UMETA(DisplayName="攻击"),
	CharaterState_AttackingNext UMETA(DisplayName="可以继续攻击"),
	CharaterState_Defending UMETA(DisplayName="防御"),
	CharaterState_BeAttack UMETA(DisplayName="受击"),
	CharaterState_Doge UMETA(DisplayName="闪避"),
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


// 玩家状态标识
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	None = 0 UMETA(DisplayName="无"),
	InPlayAttack UMETA(DisplayName="是否在攻击中"),
	InComboNext UMETA(DisplayName="是否准备播放下一个战斗"),
	CanAttack UMETA(DisplayName="可以攻击"),
	CanRecordInput UMETA(DisplayName="可以记录输入"),
	BeBlockAttack UMETA(DisplayName="被格挡攻击"),
	WuDi UMETA(DisplayName="无敌状态"),
};

// 输入标识
UENUM(BlueprintType)
enum class EInputEnum : uint8
{
    None = 0  UMETA(DisplayName="无")  ,
    MoveForward  UMETA(DisplayName="前进")  ,
    MoveBackward  UMETA(DisplayName="后退")  ,
    MoveLeft  UMETA(DisplayName="左移")  ,
    MoveRight  UMETA(DisplayName="右移")  ,
    NormalAttack  UMETA(DisplayName="轻击")  ,
    HeavyAttack  UMETA(DisplayName="重击")  ,
    Defend  UMETA(DisplayName="防御")  ,
    Doge  UMETA(DisplayName="闪避")  ,
};

USTRUCT(BlueprintType)
struct FAttackAnimTable : public  FTableRowBase
{
	GENERATED_BODY()

	// ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID = 0;

	// 输入合集
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EInputEnum> MoveInputs = {};

	// 输入合集
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EInputEnum> Inputs = {};

	// 动作对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> ActionAnimMontageInPlace = nullptr;

	// 动作对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> ActionAnimMontageMove = nullptr;

	// 消除格挡值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackBlockValue = 0;

	// 消除血量值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackHPValue = 0;
};

/**
 * 
 */
UCLASS()
class FIGHTDEMO_API UFightInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	virtual void Init() override;

	// 获取目录下所有 AnimSequence 资源路径
	TArray<FString> GetAllAnimSequencePaths(const FString& Directory, bool bRecursive);
	// 异步加载目录下所有 AnimSequence 资源
	void LoadAllAnimSequencesAsync(const FString& RootDirectory,bool bRecursive);

	UPROPERTY()
	TMap<FString, UAnimSequence*> LoadedAnims;


	UPROPERTY(EditAnywhere)
	bool IsUseCommon = true;


	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> CommonFightTable;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> GameFightTable;

	TArray<FAttackAnimTable*> AttackAnimTableArray;

	// 计算受击方向的函数
	static EHitDirection8 CalculateHitDirection8(
		const FVector& VictimLocation,
		const FRotator& VictimRotation,
		const FVector& AttackerLocation);

	UFUNCTION(BlueprintCallable, Category = "Combat|Direction")
	static EHitDirection8 CalculateHitDirection(
		AActor* Victim,
		AActor* Attacker);
};
