#pragma once

#include "ObjectType.h"

class IPoolable
{
public:
    virtual void OnEnable()                = 0;
    virtual void OnDisable()               = 0;
    virtual ObjectType GetPoolType() const = 0;
    virtual ~IPoolable()                   = default;
};
