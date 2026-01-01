#pragma once

#include <unordered_map>
#include "axmol/axmol.h"
#include "PoolInfo.h"
#include "PoolRuntime.h"

class ObjectPoolManager
{
public:
    static void Initialize();
    static ax::Node* Get(ObjectType object_type);
    static void Release(Node* object);

private:
    static void Preload(const PoolInfo& info);
    static const PoolInfo* GetPoolInfo(ObjectType type);

private:
    static std::unordered_map<ObjectType, PoolRuntime> s_pool_list;
};
