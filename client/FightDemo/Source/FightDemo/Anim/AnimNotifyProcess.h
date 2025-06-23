#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AnimNotifyProcess.generated.h"

/**
 *  CombatAttacker Interface
 *  Provides common functionality to trigger attack animation events.
 */
UINTERFACE(MinimalAPI, NotBlueprintable)
class UAnimNotifyProcess : public UInterface
{
	GENERATED_BODY()
};

class IAnimNotifyProcess
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="AnimNotify")
	virtual void DoAnimNotify(UFightAnimNotify* animNotify) = 0;

	UFUNCTION(BlueprintCallable, Category="AnimNotify")
	virtual void DoAnimNotifyState(UFightAnimNotifyState* animNotyfy, bool state) = 0;

};

