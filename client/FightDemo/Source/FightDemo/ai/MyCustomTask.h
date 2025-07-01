// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "mode/GameFightBase.h"
#include "MyCustomTask.generated.h"

USTRUCT()
struct FIGHTDEMO_API FMyCustomTaskInstanceData
{
	GENERATED_BODY()
	// 这里定义任务的实例数据（运行时状态）
	// UPROPERTY(EditAnywhere, Category=Input )
	// UPROPERTY(EditAnywhere, Category=Output )
	// UPROPERTY(EditAnywhere, Category=Parameter )
	// UPROPERTY(EditAnywhere, Category=Context )

	// 这里可以定义任务的配置属性（参数）
	UPROPERTY(EditAnywhere, Category=Parameter)
	FString TaskName; // 示例数据，可以根据需要添加更多字段

	// 运行时间
	UPROPERTY(EditAnywhere, Category=Output)
	FString TaskTime;

	// 任务执行时的拥有者（通常是一个角色或AI控制器）
	UPROPERTY(EditAnywhere, Category=Context)
	AGameFightBase* OwnActor = nullptr;

	// 输入参数
	UPROPERTY(EditAnywhere, Category=Input)
	float ActorDistance;
};

USTRUCT(meta = (DisplayName = "My Custom Task", Category = "Game"))
struct FIGHTDEMO_API FMyCustomTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FMyCustomTaskInstanceData; // 关联实例数据类型

	FMyCustomTask();

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	// 任务执行的生命周期函数
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	//virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	//virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

};

