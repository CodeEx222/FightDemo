//

#pragma once

#include "CoreMinimal.h"
#include <bitset>
#include <memory>

#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "FightDemo/mode/FightInstance.h"
#include "FightComponent.generated.h"

struct FInputElement;
class UGameAnimInstance;
class AGameFightCharacter;

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
	void AddInput(EInputEnum InputEnum);


	void PlaySkill(FAttackAnimTable* SkillToPlay);
	void PlayBeAttackSkill(AGameFightCharacter* AttackActor ,FGameplayTag AttackTag);
	void PlayBlockAttackSkill(AGameFightCharacter* AttackActor ,FGameplayTag AttackTag);

	virtual void OnAnimNotify(UAnimNotify * Notify);
	virtual void OnAnimNotifyState(UAnimNotifyState * NotifyState, bool bStart);



	void AddNewInput(const FInputElement& NewInput);


	void SetPlayerActionState(EPlayerState Type , bool Value = true) const;
	bool GetPlayerActionState(EPlayerState Type) const;

	// 攻击玩家
	AGameFightCharacter* GetAttackCharacter(bool& OutIsMove);


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

	// 播放受击
	void PlayHit(EHitDirection8 AttackerDir, FGameplayTag AttackTag, bool bIsMove);
	void PlayBlock(EHitDirection8 AttackerDir, FGameplayTag AttackTag, bool bIsMove);
};
