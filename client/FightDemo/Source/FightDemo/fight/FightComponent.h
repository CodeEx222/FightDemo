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


	void PlayAttackSkill(FAttackAnimTable* SkillToPlay);
	void PlayBeAttackSkill(AGameFightBase* AttackActor ,FGameplayTag AttackTag);
	void PlayBlockBeAttack(AGameFightBase* AttackActor ,FGameplayTag AttackTag);
	void PlayBlockBreak(AGameFightBase* AttackActor ,FGameplayTag AttackTag);

	virtual void OnAnimNotify(UAnimNotify * Notify);
	virtual void OnAnimNotifyState(UAnimNotifyState * NotifyState, bool bStart);



	void AddNewInput(const FInputElement& NewInput);


	// 攻击玩家
	AGameFightBase* GetAttackCharacter(bool& OutIsMove);


	// 闪避
	void PlayDoge();

	UFUNCTION(BlueprintCallable)
	void PlayBlock(bool EnterValue);


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
	FAttackAnimTable* CurrentAnimTable;

	// 动画播放实例ID
	int AnimPlayInstanceID;

	UFUNCTION()
	void CharacterPlayMontage(UAnimMontage* Montage, float InPlayRate = 1.0f, FName StartSectionName = NAME_None,
		EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength);

	UFUNCTION()
	void OnMontagePlayBlendingOut(UAnimMontage* Montage, bool bInterrupted, int32 InstanceID);

	// 播放受击
	void PlayHit(EHitDirection8 AttackerDir, FGameplayTag AttackTag);
	void PlayBlockHit(EHitDirection8 AttackerDir, FGameplayTag AttackTag);
};
