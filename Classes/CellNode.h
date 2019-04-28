//
// Created by ondesly on 2019-04-27.
//

#pragma once

#include <2d/CCNode.h>

#include "Thing.h"

namespace fm {

    class CellNode : public cocos2d::Node {
    public:

        static CellNode *create(size_t index, const cocos2d::Size &size);

    public:

        bool init(const cocos2d::Size &size);

        size_t getIndex() const;

        void setThing(const Thing &thing);

        const Thing &getThing() const;

    private:

        const size_t mIndex;

        Thing mThing;

    private:

        CellNode(size_t index);

    };

}