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
