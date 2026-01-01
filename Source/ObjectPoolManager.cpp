#include "ObjectPoolManager.h"
#include "ObjectFactory.h"
#include "IPoolable.h"

USING_NS_AX;

std::unordered_map<ObjectType, PoolRuntime> ObjectPoolManager::s_pool_list;

void ObjectPoolManager::Initialize()
{
    for (const PoolInfo& info : POOL_INFO_TABLE)
        Preload(info);
}

Node* ObjectPoolManager::Get(ObjectType object_type)
{
    const PoolInfo* info = GetPoolInfo(object_type);
    auto& runtime = s_pool_list[object_type];

    Node* object = nullptr;
    if (!runtime.m_pool.empty())
    {
        object = runtime.m_pool.back();
        runtime.m_pool.pop_back();
    }
    else if (info && info->m_can_expand && runtime.m_current_count < info->m_max_count)
    {
        object = ObjectFactoryC::Create(object_type);
        runtime.m_current_count++;
    }
    else
    {
        AXLOGD("ObjectPoolManager: 풀이 가득 찼습니다.");
        return nullptr;
    }

    auto poolable = dynamic_cast<IPoolable*>(object);
    if (poolable)
        poolable->OnEnable();
    
    return object;
}

void ObjectPoolManager::Release(Node* object)
{
    auto poolable = dynamic_cast<IPoolable*>(object);
    if (!poolable)
        return;

    poolable->OnDisable();
    object->removeFromParent();

    ObjectType type = poolable->GetPoolType();
    s_pool_list[type].m_pool.push_back(object);
}

void ObjectPoolManager::Preload(const PoolInfo& info)
{
    auto& runtime = s_pool_list[info.m_type];

    for (int i = 0; i < info.m_initial_count; i++)
    {
        Node* object = ObjectFactoryC::Create(info.m_type);
        if (!object)
            continue;

        auto poolable = dynamic_cast<IPoolable*>(object);
        if (poolable)
            poolable->OnDisable();

        runtime.m_pool.push_back(object);
        runtime.m_current_count++;
    }
}

const PoolInfo* ObjectPoolManager::GetPoolInfo(ObjectType type)
{
    for (const PoolInfo& info : POOL_INFO_TABLE)
    {
        if (info.m_type == type)
            return &info;
    }

    return nullptr;
}
