#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameAnimInstance.generated.h"

class AGameFightCharacter;
/**
 * 
 */
UCLASS()
class FIGHTDEMO_API UGameAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	AGameFightCharacter* gameCharacter;


	UFUNCTION(BlueprintImplementableEvent)
	void PlayFightMontage(UAnimMontage* AnimMontage, float PlayRate = 1.0, float StartPosition = 0.0f,
	                      const FName &NotifyName = FName("None"), bool bStopAllMontages = true);
};
