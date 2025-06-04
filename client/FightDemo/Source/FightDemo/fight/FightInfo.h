#pragma once

#include "CoreMinimal.h"

#include <queue>
#include <set>

#include "FightDemo/util/GameObjectMemoryPool.h"
#include "FightInfo.generated.h"

UENUM(Blueprintable)
enum class EActionLineState : uint8
{
	None = 0 UMETA(DisplayName="无"),
	SkillStart = 1 UMETA(DisplayName="技能开始"),
};

UCLASS(Blueprintable)
class UFBaseTimeNode : public UObject
{
	GENERATED_BODY()
public:
	int NodeId = 0;

	// 作用时间
	int64 ActionTime;
	// 技能作用类型
	EActionLineState ActionLineState;

	std::vector<std::weak_ptr<UFBaseTimeNode>>* Children;

	virtual void ResetData();

	virtual void CopyData(const UFBaseTimeNode& OBJ, const bool bCopyId = false);

	virtual FString ToString() const;
};

struct CompareSkillUseInfo
{
	bool operator ()(const std::shared_ptr<UFBaseTimeNode>& P1, const std::shared_ptr<UFBaseTimeNode>& P2) const
	{
		return (P1->ActionTime * 1000 + static_cast<int64>(P1->ActionLineState)) <
			(P2->ActionTime * 1000 + static_cast<int64>(P2->ActionLineState));
	}
};

static class NodeIdGenFactory
{
public:
	static int NodeIdIndex;

	static uint64 GenId()
	{
		NodeIdIndex++;
		return NodeIdIndex;
	}
};


template< class T> requires std::derived_from<T, UFBaseTimeNode>
class TFightTimeLine
{
public:

	TFightTimeLine()
	{
		this->NodePool = new TFGameObjectMemoryPool<T>(NodeIdGenFactory::GenId);
	}

	std::shared_ptr<T> GetFreeObj(bool bGenId);
	bool Add(std::shared_ptr<UFBaseTimeNode> OBJ);

	/**
	 * 获取当前时间线的对象数量
	 * @return 数量
	 */
	uint32_t GetSize() const;


	/**
	 * 获取当前时间线最前的动作Node
	 * @return 动作NODE
	 */
	std::shared_ptr<T> GetFront();


	/**
	 * 获取当前时间线指定的动作Node
	 * @param id  动作ID
	 * @return 动作NODE
	 */
	std::shared_ptr<T> GetById(uint64 id);
	void PopById(uint64 ID);

private:
	std::unordered_map<int32_t, std::shared_ptr<T>> NodeDataMap;

	// 当前排序列表
	std::multiset<std::weak_ptr<T>, CompareSkillUseInfo> SortNodeList;

	// node空闲对象池
	TFGameObjectMemoryPool<T>* NodePool;
};

template <class T> requires std::derived_from<T, UFBaseTimeNode>
std::shared_ptr<T> TFightTimeLine<T>::GetFreeObj(bool bGenId)
{
	if (NodePool != nullptr)
	{
		return NodePool.GetFreeObj(bGenId);
	}
	return nullptr;
}

template <class T> requires std::derived_from<T, UFBaseTimeNode>
uint32_t TFightTimeLine<T>::GetSize() const
{
	return NodeDataMap.size();
}

template <class T> requires std::derived_from<T, UFBaseTimeNode>
std::shared_ptr<T> TFightTimeLine<T>::GetFront()
{
	auto Iter = SortNodeList.begin();
	while (Iter != SortNodeList.end())
	{
		if (Iter->expired())
		{
			// 资源不存在了
			// 如果没有找到, 说明这个对象已经被删除了
			Iter = SortNodeList.erase(Iter);
		}
		else
		{
			auto OBJ = Iter->lock();
			if (NodeDataMap.contains(OBJ->NodeId))
			{
				return NodeDataMap[OBJ->NodeId];
			}
		}
	}
	return nullptr;
}

template <class T> requires std::derived_from<T, UFBaseTimeNode>
std::shared_ptr<T> TFightTimeLine<T>::GetById(uint64 id)
{
	if (const auto Find_Iter = NodeDataMap.find(id); Find_Iter != NodeDataMap.end())
	{
		auto OBJ = NodeDataMap[id];
		return OBJ;
	}

	return nullptr;
}

template <class T> requires std::derived_from<T, UFBaseTimeNode>
void TFightTimeLine<T>::PopById(uint64 ID)
{
	auto FindIter = NodeDataMap.find(ID);
	if (FindIter != NodeDataMap.end())
	{
		NodeDataMap.erase(FindIter);

		if (NodePool != nullptr)
		{
			NodePool->PushFreeObj(FindIter->second);
		}

	}
}

template <class T> requires std::derived_from<T, UFBaseTimeNode>
bool TFightTimeLine<T>::Add(std::shared_ptr<UFBaseTimeNode> OBJ)
{
	if (NodeDataMap.contains(OBJ->NodeId))
	{
		return false;
	}

	SortNodeList.insert(OBJ);
	NodeDataMap.emplace(OBJ->NodeId, OBJ);

	return true;
}


// 记录技能使用结构
UCLASS(Blueprintable)
class USkillActionInfo : public UFBaseTimeNode
{
	GENERATED_BODY()
public:
	virtual void ResetData() override
	{
		UFBaseTimeNode::ResetData();
	}

	virtual void CopyData(const UFBaseTimeNode& OBJ, const bool bCopyId = false) override
	{
		UFBaseTimeNode::CopyData(OBJ, bCopyId);
	}

	virtual FString ToString() const override
	{
		const auto BaseInfo = UFBaseTimeNode::ToString();
		return BaseInfo + FString::Format(
		TEXT("%s"),
		{ "" }
	);
	}

};