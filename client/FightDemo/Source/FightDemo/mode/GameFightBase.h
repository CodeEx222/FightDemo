// 

#pragma once

#include "CoreMinimal.h"
#include "Anim/AnimNotifyProcess.h"
#include "GameFramework/Character.h"
#include "GameFightBase.generated.h"

class UFightAnimNotifyState;
class UFightAnimNotify;
class UPlayerAttributeComponent;
class UFightComponent;

UCLASS()
class FIGHTDEMO_API AGameFightBase  : public ACharacter, public IAnimNotifyProcess
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameFightBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// 战斗组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	UFightComponent* FightComponent;

	// 玩家属性组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	UPlayerAttributeComponent* PlayerAttributeComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 处理移动输入
	UFUNCTION(BlueprintCallable)
	void MoveToDir(FRotator MoveDir, float Right, float Forward);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGameFightBase* FightTarget;

	UFUNCTION(BlueprintCallable)
	void ChangeTarget();

	UFUNCTION(BlueprintCallable)
	void ClearTarget();


	UFUNCTION(BlueprintCallable, Category="AnimNotify")
	virtual void DoAnimNotify(UFightAnimNotify* AnimNotify) override;

	UFUNCTION(BlueprintCallable, Category="AnimNotify")
	virtual void DoAnimNotifyState(UFightAnimNotifyState* AnimNotyfy, bool bState) override;
};
