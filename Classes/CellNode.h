//
// Created by ondesly on 2019-04-27.
//

#pragma once

#include <2d/CCNode.h>

#include "Thing.h"

namespace fm {

    class CellNode : public cocos2d::Node {
    public:

        static CellNode *create(const cocos2d::Size &size);

    public:

        bool init(const cocos2d::Size &size);

        void setThing(const Thing &thing);

        const Thing &getThing() const;

    private:

        Thing mThing;

    };

}