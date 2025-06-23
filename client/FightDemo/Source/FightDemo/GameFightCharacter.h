

#pragma once

#include "CoreMinimal.h"
#include "Anim/AnimNotifyProcess.h"
#include "mode/FightInstance.h"
#include "GameFramework/Character.h"
#include "GameFightCharacter.generated.h"


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	UFightComponent* FightComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	UProcessInputComponent* ProcessInputComponent;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGameFightCharacter* PlayerFightTarget;




	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "连击数量"))
	int AttackNum;

	bool canFanJi;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "左右移动值"))
	float MoveActionX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "前后移动值"))
	float MoveActionY;

	UFUNCTION(BlueprintCallable)
	void UpdateActorRotator();


	UFUNCTION(BlueprintImplementableEvent)
	void ChangeTarget();

	//UFUNCTION(BlueprintCallable)
	//void Anim_Notify(EAnimNotifyState notifyState);


	UFUNCTION(BlueprintImplementableEvent)
	void ChangeTime();


	bool IsAttackPlayer(AGameFightCharacter* target);
	// 处理移动输入
	void DoMove(float Right, float Forward);
	// 处理视角输入
	void DoLook(float Yaw, float Pitch);


	UFUNCTION(BlueprintImplementableEvent)
	void HpChangeView(float HpValue,float MaxValue);

	UFUNCTION(BlueprintImplementableEvent)
	void BlockChangeView(float HpValue,float MaxValue);


	UFUNCTION(BlueprintCallable, Category="AnimNotify")
	virtual void DoAnimNotify(UFightAnimNotify* animNotify) override;

	UFUNCTION(BlueprintCallable, Category="AnimNotify")
	virtual void DoAnimNotifyState(UFightAnimNotifyState* animNotyfy, bool state) override;


};


