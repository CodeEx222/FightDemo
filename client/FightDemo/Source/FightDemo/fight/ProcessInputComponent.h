//

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "FightDemo/mode/FightInstance.h"
#include "ProcessInputComponent.generated.h"

USTRUCT(BlueprintType)
struct FInputElement
{
	GENERATED_BODY()
public:

	// 保存输入的枚举
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInputEnum InputEnum = EInputEnum::None;

	// 保存输入的时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	double GameTime = 0.0;

	// 是否是新的输入,不是老的输入,防止反复出发
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool IsNewCheck = true;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIGHTDEMO_API UProcessInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UProcessInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere, Category ="Input", Meta = (DisplayName = "攻击输入"))
	UInputAction* InputAttack;

	UPROPERTY(EditAnywhere, Category ="Input", Meta = (DisplayName = "攻击输入(重击)"))
	UInputAction* InputAttackHeavy;

	UPROPERTY(EditAnywhere, Category ="Input" , Meta = (DisplayName = "防御输入"))
	UInputAction* InputBlock;

	UPROPERTY(EditAnywhere, Category ="Input" , Meta = (DisplayName = "切换目标"))
	UInputAction* InputChangeTarget;

	UPROPERTY(EditAnywhere, Category ="Input" , Meta = (DisplayName = "闪避输入"))
	UInputAction* InputDoge;

	UPROPERTY(EditAnywhere, Category ="Input", Meta = (DisplayName = "摄像头移动"))
	UInputAction* InputLook;

	UPROPERTY(EditAnywhere, Category ="Input" , Meta = (DisplayName = "移动输入"))
	UInputAction* InputMove;

	UPROPERTY(EditAnywhere, Category ="Input" , Meta = (DisplayName = "移动输入(前)"))
	UInputAction* InputMoveF;

	UPROPERTY(EditAnywhere, Category ="Input" , Meta = (DisplayName = "移动输入(后)"))
	UInputAction* InputMoveB;

	UPROPERTY(EditAnywhere, Category ="Input" , Meta = (DisplayName = "移动输入(左)"))
	UInputAction* InputMoveL;

	UPROPERTY(EditAnywhere, Category ="Input" , Meta = (DisplayName = "移动输入(右)"))
	UInputAction* InputMoveR;

	void ProcessInputAttack(const FInputActionValue& Value);
	void ProcessInputAttackHeavy(const FInputActionValue& Value);
	void ProcessInputDoge(const FInputActionValue& Value);
	void ProcessInputLook(const FInputActionValue& Value);
	void ProcessInputMove(const FInputActionValue& Value);
	void ProcessInputMoveF(const FInputActionValue& Value);
	void ProcessInputMoveB(const FInputActionValue& Value);
	void ProcessInputMoveL(const FInputActionValue& Value);
	void ProcessInputMoveR(const FInputActionValue& Value);

	void ProcessInputBlockPressed(const FInputActionValue& Value);
	void ProcessInputChangeTargetPressed(const FInputActionValue& Value);

	void ProcessInputBlockReleased(const FInputActionValue& Value);
	void ProcessInputChangeTargetLongPressed(const FInputActionValue& Value);

	double StartPressTime = 0.0f;

};
