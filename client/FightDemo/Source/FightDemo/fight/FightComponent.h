//

#pragma once

#include "CoreMinimal.h"
#include <bitset>
#include "FightInfo.h"
#include "Components/ActorComponent.h"
#include "FightDemo/game/FightInstance.h"
#include "FightComponent.generated.h"

enum class EInputEnum : uint8;





USTRUCT(BlueprintType)
struct FInputElement
{
	GENERATED_BODY()
public:

	// 保存输入的枚举
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInputEnum InputEnum;

	// 保存输入的时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	double GameTime;

	// 是否是新的输入,不是老的输入,防止反复出发
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool IsNewCheck;
};


// 记录技能使用结构
UCLASS(Blueprintable)
class USkillActionInfo : public UFBaseTimeNode
{
	GENERATED_BODY()
public:
	virtual void ResetData() override
	{
		UFBaseTimeNode::ResetData();
	}

	virtual void CopyData(const UFBaseTimeNode& OBJ, const bool bCopyId = false) override
	{
		UFBaseTimeNode::CopyData(OBJ, bCopyId);
	}

	virtual FString ToString() const override
	{
		const auto BaseInfo = UFBaseTimeNode::ToString();
		return BaseInfo + FString::Format(
		TEXT("%s"),
		{ "" }
	);
	}

};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIGHTDEMO_API UFightComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// 记录输入记录
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInputElement> InputArray;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInputElement> MoveInputArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "血量"))
	int Hp = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "格挡值"))
	int BlockValue = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "人物状态"))
	ECharaterState GameCharaterState;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UFightComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void CheckActionTimeLine(int64 CurrentTime, float DeltaTime) const;

	UFUNCTION(BlueprintCallable)
	void AddInput(EInputEnum inputEnum);


	void PlaySkill(FAttackAnimTable* SkillToPlay);

	virtual void OnAnimNotify(UAnimNotify * Notify);
	virtual void OnAnimNotifyState(UAnimNotifyState * NotifyState, bool bStart);



	void AddNewInput(const FInputElement& NewInput);


	void SetPlayerActionState(EPlayerState Type , bool Value = true) const;
	bool GetPlayerActionState(EPlayerState Type) const;


private:
	void CheckAttack();
	FAttackAnimTable* CheckInput();

	// 动作状态指示
	std::shared_ptr<std::bitset<32>> PlayerActionStateBitset;

	// 保存 行动时间线  结束时间线  行动结构内存池的结构
	TFightTimeLine<USkillActionInfo>* FightTimeLineObj;

	UDataTable* SkillDataTable;

	TArray<FAttackAnimTable*> AttackAnimTableArray;

	UAnimMontage* MyMontage1;
	UAnimMontage* MyMontage2;
	UAnimMontage* MyMontage3;
	UAnimMontage* MyMontage4;
};
