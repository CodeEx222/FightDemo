

#pragma once

#include "CoreMinimal.h"
#include "game/FightInstance.h"
#include "GameFramework/Character.h"
#include "GameFightCharacter.generated.h"


class UFightComponent;
enum class EAnimNotifyState : uint8;
enum class EInputEnum : uint8;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	UFightComponent* FightComponent;

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

	//UFUNCTION(BlueprintCallable)
	//void Anim_Notify(EAnimNotifyState notifyState);


	UFUNCTION(BlueprintImplementableEvent)
	void ChangeTime();


	bool IsAttackPlayer(AGameFightCharacter* target);

};


