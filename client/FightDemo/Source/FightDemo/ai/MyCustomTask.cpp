//


#include "MyCustomTask.h"

#include "StateTreeExecutionContext.h"

FMyCustomTask::FMyCustomTask()
{
	// 是否调用tick
	bShouldCallTick = false;
}

EStateTreeRunStatus FMyCustomTask::EnterState(FStateTreeExecutionContext& Context,
                                              const FStateTreeTransitionResult& Transition) const
{
	// 获取实例数据 注意返回引用才能更改里面的值
	// FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	//Do something with InstanceData

	return FStateTreeTaskBase::EnterState(Context, Transition);
}

// EStateTreeRunStatus FMyCustomTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
// {
// 	return FStateTreeTaskBase::Tick(Context, DeltaTime);
// }

// void FMyCustomTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
// {
// 	FStateTreeTaskBase::ExitState(Context, Transition);
// }

