//

#pragma once

#include "CoreMinimal.h"
#include <bitset>
#include "FightInfo.h"
#include "Components/ActorComponent.h"
#include "FightDemo/mode/FightInstance.h"
#include "FightComponent.generated.h"

class UGameAnimInstance;
class AGameFightCharacter;
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

USTRUCT(BlueprintType)
struct FCharacterAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double MaxValue;

	// 回复速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double RecoverValue;

	// 格挡恢复开始时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double RecoverTime;

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
	FCharacterAbility HPValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "格挡值"))
	FCharacterAbility BlockValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "人物状态"))
	ECharaterState GameCharaterState;

	// 闪避对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> DogeAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "格挡受击动画"))
	TSoftObjectPtr<UAnimMontage> BlockBeAttackAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "结束攻击动画"))
	TSoftObjectPtr<UAnimMontage> EndAttackAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "结束受击动画"))
	TSoftObjectPtr<UAnimMontage> EndBeAttackAnimMontage;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UFightComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddInput(EInputEnum inputEnum);


	void PlaySkill(FAttackAnimTable* SkillToPlay);
	void PlayBeAttackSkill(FAttackAnimTable* SkillToPlay);
	void PlayBlockAttackSkill(FAttackAnimTable* SkillToPlay);

	virtual void OnAnimNotify(UAnimNotify * Notify);
	virtual void OnAnimNotifyState(UAnimNotifyState * NotifyState, bool bStart);



	void AddNewInput(const FInputElement& NewInput);


	void SetPlayerActionState(EPlayerState Type , bool Value = true) const;
	bool GetPlayerActionState(EPlayerState Type) const;

	// 攻击玩家
	AGameFightCharacter* GetAttackCharacter();


	// 闪避
	void PlayDoge();
	void PlayBlockBeAttack();

private:
	void CheckAttack();
	FAttackAnimTable* CheckInput();

	// 动作状态指示
	std::shared_ptr<std::bitset<32>> PlayerActionStateBitset;

	UPROPERTY()
	AGameFightCharacter* OwnCharacterPtr;

	AGameFightCharacter* GetOwnCharacter();

	UPROPERTY()
	UGameAnimInstance* OwnAnimInstance;
	UGameAnimInstance* GetAnimInstance();

	// 当前播放的动作表
	FAttackAnimTable* CurrentAnimTable;

	// 动画播放实例ID
	int AnimPlayInstanceID;

	UFUNCTION()
	void CharacterPlayMontage(UAnimMontage* Montage, float InPlayRate = 1.0f, FName StartSectionName = NAME_None,
		EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength);

	UFUNCTION()
	void OnMontagePlayBlendingOut(UAnimMontage* Montage, bool bInterrupted, int32 InstanceID);
};
