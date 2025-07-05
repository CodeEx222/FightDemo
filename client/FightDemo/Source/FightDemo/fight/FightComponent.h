//

#pragma once

#include "CoreMinimal.h"
#include <bitset>
#include <memory>

#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "FightDemo/mode/FightInstance.h"
#include "FightComponent.generated.h"

class AGameFightBase;
struct FInputElement;
class UGameAnimInstance;
class AGameFightCharacter;


USTRUCT(BlueprintType)
struct FPlayAttackData
{
	GENERATED_BODY()

public:

	// 动作对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ActionAnimMontageInPlace = nullptr;

	// 动作对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ActionAnimMontageMove = nullptr;

	// 消除格挡值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackBlockValue = 0;

	// 消除血量值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackHPValue = 0;

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

	//当前已拥有的Tag
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	//当前互斥的Tag
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag ActiveMutexGameplayTags;

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
	void AddInput(EInputEnum InputEnum);


	UFUNCTION(BlueprintCallable)
	float PlayAttackSkill(FAttackAnimTable SkillToPlay);

	UFUNCTION(BlueprintCallable)
	float PlayAttackSkillByData(FPlayAttackData SkillToPlay);


	void PlayBeAttackSkill(AGameFightBase* AttackActor ,FGameplayTag AttackTag);
	void PlayBlockBeAttack(AGameFightBase* AttackActor ,FGameplayTag AttackTag);
	void PlayBlockBreak(AGameFightBase* AttackActor ,FGameplayTag AttackTag);
	void PlayFinish(AGameFightBase* AttackActor);

	virtual void OnAnimNotify(UAnimNotify * Notify);
	virtual void OnAnimNotifyState(UAnimNotifyState * NotifyState, bool bStart);



	void AddNewInput(const FInputElement& NewInput);


	// 攻击玩家
	AGameFightBase* GetAttackCharacter(bool& OutIsMove);


	// 闪避
	void PlayDoge();

	UFUNCTION(BlueprintCallable)
	void PlayBlock(bool EnterValue);

	UFUNCTION()
	float CharacterPlayMontage(UAnimMontage* Montage, float InPlayRate = 1.0f, FName StartSectionName = NAME_None,
		EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength);


	// 当前播放的反击动作
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayAttackData BlockAttackData;

private:
	void CheckAttack();
	FAttackAnimTable* CheckInput();

	UPROPERTY()
	AGameFightBase* OwnCharacterPtr;

	AGameFightBase* GetOwnCharacter();

	UPROPERTY()
	UGameAnimInstance* OwnAnimInstance;
	UGameAnimInstance* GetAnimInstance();

	// 当前播放的动作表
	FPlayAttackData CurrentAnimTable;

	// 动画播放实例ID
	int AnimPlayInstanceID;

	FTimerHandle PlayMontageTimerHandle;



	UFUNCTION()
	void OnMontagePlayBlendingOut(UAnimMontage* Montage, bool bInterrupted, int32 InstanceID);

	void OnAnimPlayFinish();
	// 播放受击
	void PlayHit(EHitDirection8 AttackerDir, FGameplayTag AttackTag);
	void PlayBlockHit(EHitDirection8 AttackerDir, FGameplayTag AttackTag);
};
