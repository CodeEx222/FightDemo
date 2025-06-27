// 

#pragma once

#include "CoreMinimal.h"
#include "GameFightBase.h"
#include "Anim/AnimNotifyProcess.h"
#include "GameFramework/Character.h"
#include "GameFightNpc.generated.h"

UCLASS()
class FIGHTDEMO_API AGameFightNpc : public AGameFightBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameFightNpc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
