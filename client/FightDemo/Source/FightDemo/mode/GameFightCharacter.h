

#pragma once

#include "CoreMinimal.h"
#include "GameFightBase.h"
#include "Components/WidgetComponent.h"
#include "GameFightCharacter.generated.h"


class UProcessInputComponent;

UCLASS(Blueprintable)
class FIGHTDEMO_API AGameFightCharacter : public AGameFightBase
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

	// 输入组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	UProcessInputComponent* ProcessInputComponent;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void UpdateActorRotator();


	// 玩家头显UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	UWidgetComponent* HeadViewUI;


	bool IsAttackPlayer(AGameFightCharacter* Target);
	// 处理移动输入
	UFUNCTION(BlueprintCallable)
	void DoMove(float Right, float Forward);
	// 处理视角输入
	UFUNCTION(BlueprintCallable)
	void DoLook(float Yaw, float Pitch);

};


