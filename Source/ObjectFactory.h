#pragma once

#include "axmol/axmol.h"
#include "ObjectType.h"
#include "Mong.h"
#include "FloatingText.h"

USING_NS_AX;

class ObjectFactoryC
{
public:
    static Node* Create(ObjectType object_type)
    {
        Node* object = nullptr;
        switch (object_type)
        {
        case ObjectType::MONG:
            object = Mong::create();
            break;

        case ObjectType::FLOATING_TEXT:
            object = FloatingText::create();
            break;

        default:
            AXLOGD("ObjectFactory: 올바른 오브젝트 타입을 입력하세요.");
            return nullptr;
        }

        if (object)
            object->retain();

        return object;
    }
};
