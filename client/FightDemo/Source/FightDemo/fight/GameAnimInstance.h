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

	UFUNCTION()
	float PlayAnimMontage(class UAnimMontage* AnimMontage,
		float InPlayRate = 1.0f, FName StartSectionName = NAME_None,
		EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength);
};
