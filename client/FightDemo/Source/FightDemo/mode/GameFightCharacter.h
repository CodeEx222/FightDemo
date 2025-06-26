

#pragma once

#include "CoreMinimal.h"
#include "FightDemo/Anim/AnimNotifyProcess.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameFightCharacter.generated.h"


class UFightAnimNotifyState;
class UFightAnimNotify;
class UHeadView;
class UPlayerAttributeComponent;
class UProcessInputComponent;
class UFightComponent;
enum class EAnimNotifyState : uint8;
enum class EInputEnum : uint8;

UCLASS(Blueprintable)
class FIGHTDEMO_API AGameFightCharacter : public ACharacter, public IAnimNotifyProcess
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameFightCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 战斗组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	UFightComponent* FightComponent;

	// 输入组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	UProcessInputComponent* ProcessInputComponent;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 玩家属性组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	UPlayerAttributeComponent* PlayerAttributeComponent;

	// 玩家头显UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HeadViewUI;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGameFightCharacter* PlayerFightTarget;




	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "连击数量"))
	int AttackNum;

	bool canFanJi;

	UFUNCTION(BlueprintCallable)
	void UpdateActorRotator();


	UFUNCTION(BlueprintCallable)
	void ChangeTarget();

	UFUNCTION(BlueprintCallable)
	void ClearTarget();


	bool IsAttackPlayer(AGameFightCharacter* Target);
	// 处理移动输入
	UFUNCTION(BlueprintCallable)
	void DoMove(float Right, float Forward);
	// 处理视角输入
	UFUNCTION(BlueprintCallable)
	void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintCallable, Category="AnimNotify")
	virtual void DoAnimNotify(UFightAnimNotify* AnimNotify) override;

	UFUNCTION(BlueprintCallable, Category="AnimNotify")
	virtual void DoAnimNotifyState(UFightAnimNotifyState* AnimNotyfy, bool bState) override;

};


