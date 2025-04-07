#include "FightInfo.h"

int NodeIdGenFactory::NodeIdIndex = 0;

void UFBaseTimeNode::ResetData()
{
	if (Children != nullptr)
	{
		Children->clear();
		delete Children;
		Children = nullptr;
	}
}

void UFBaseTimeNode::CopyData(const UFBaseTimeNode& OBJ, const bool bCopyId)
{
	if (bCopyId)
	{
		NodeId = OBJ.NodeId;
	}
	ActionTime = OBJ.ActionTime;
	ActionLineState = OBJ.ActionLineState;
	Children = OBJ.Children;
}

FString UFBaseTimeNode::ToString() const
{
	return FString::Format(
		TEXT("NodeId:%d ActionTime:%lld ActionLineState:%s"),
		{ NodeId, ActionTime, UEnum::GetDisplayValueAsText(ActionLineState).ToString() }
	);
}



