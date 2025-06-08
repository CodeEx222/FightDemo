#pragma once
#include <queue>
#include <functional>
#include <memory>


/**
 * @brief  游戏对象内存池
 * @tparam T 对象类型
 */
template <class T>
class TFGameObjectMemoryPool
{
public:
	explicit TFGameObjectMemoryPool(const std::function<uint64()>& Fun);
	std::shared_ptr<T> GetFreeObj(bool bGenId = true);
	bool PushFreeObj(std::shared_ptr<T> OBJ);

private:
	// 空余对象列表
	std::queue<std::shared_ptr<T>> EmptyObjPool;

	std::function<uint64()> GenIDFun;
};

/**
 * @brief  游戏对象内存池
 * @param Fun 生成ID的函数
 */
template <class T>
TFGameObjectMemoryPool<T>::TFGameObjectMemoryPool(const std::function<uint64()>& Fun)
{
	GenIDFun = Fun;
}

/**
 * @brief  释放对象到内存池
 * @param OBJ 需要释放的对象
 * @return 是否成功
 */
template <class T>
bool TFGameObjectMemoryPool<T>::PushFreeObj(std::shared_ptr<T> OBJ)
{
	OBJ->ResetData();
	EmptyObjPool.push(OBJ);
	return true;
}

/**
 * @brief  获取空闲对象
 * @param bGenId 是否生成ID
 * @return 对象指针
 */
template <class T>
std::shared_ptr<T> TFGameObjectMemoryPool<T>::GetFreeObj(const bool bGenId)
{
	std::shared_ptr<T> Result;
	if (!EmptyObjPool.empty())
	{
		Result = EmptyObjPool.front();
		EmptyObjPool.pop();
	}
	else
	{
		Result = std::make_shared<T>();
	}

	if (GenIDFun && bGenId)
	{
		Result->action_id_ = GenIDFun();
	}
	return Result;
}
