#pragma once

#include "axmol/axmol.h"

USING_NS_AX;

template<typename T>
class Singleton : public Node
{
public:
    static T* getInstance()
    {
        static T* s_instance = nullptr;

        if (!s_instance)
        {
            s_instance = static_cast<T*>(T::create());
            s_instance->retain();
        }

        return s_instance;
    }

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;
};
