//
// Created by ondesly on 2019-04-27.
//

#pragma once

#include <functional>

#include <2d/CCNode.h>

#include "Thing.h"

namespace fm {

    class ThingNode;

    class CellNode : public cocos2d::Node {
    public:

        enum class Direction {
            NONE,
            LEFT,
            TOP,
            RIGHT,
            BOTTOM
        };

    public:

        static CellNode *create(size_t index, const cocos2d::Size &size, const std::function<void(size_t, Direction)> &onHit);

    public:

        bool init(const cocos2d::Size &size);

        size_t getIndex() const;

        void setThingNode(ThingNode *thingNode);

        ThingNode *getThingNode() const;

        Thing getThing() const;

    private:

        static const float THING_SIZE_RATIO;

    private:

        const size_t mIndex;
        const std::function<void(size_t, Direction)> mOnHit;

        ThingNode *mThingNode = nullptr;

    private:

        CellNode(size_t index, const std::function<void(size_t, Direction)> &onHit);

    private:

        void onThingNodePositionChanged(ThingNode *node);

    };

}