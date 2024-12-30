

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFightCharacter.generated.h"


enum class EAnimNotifyState : uint8;
enum class EInputEnum : uint8;
enum class ECharaterState : uint8;

UCLASS(Blueprintable)
class FIGHTDEMO_API AGameFightCharacter : public ACharacter
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> FightAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	TArray<UAnimMontage*> FightAttackAnimArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	TArray<UAnimMontage*> FightBeAttackAnimArray;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGameFightCharacter* PlayerFightTarget;


	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "血量"))
	int Hp = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "格挡值"))
	int BlockValue = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "人物状态"))
	ECharaterState GameCharaterState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Meta = (DisplayName = "连击数量"))
	int AttackNum;

	bool canInputRecord;
	EInputEnum AllInputs;

	bool canFanJi;

	UFUNCTION(BlueprintCallable)
	void EnterBlock();
	UFUNCTION(BlueprintCallable)
	void ExitBlock();

	UFUNCTION(BlueprintCallable)
	void AttackPlayer();

	UFUNCTION(BlueprintCallable)
	void BeAttackPlayer(int mode);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayGameMontage(UAnimMontage* playmontage);

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeTarget();

	UFUNCTION(BlueprintCallable)
	void MontagePlayerEnd();

	UFUNCTION(BlueprintCallable)
	void Anim_Notify(EAnimNotifyState notifyState);


	UFUNCTION(BlueprintImplementableEvent)
	void ChangeTime();


	bool IsAttackPlayer(AGameFightCharacter* target);

};


