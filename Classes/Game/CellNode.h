//
// Created by ondesly on 2019-04-27.
//

#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include <2d/CCNode.h>
#include <math/CCGeometry.h>

#include "Thing.h"

namespace cocos2d {

    class EventListenerCustom;

    class EventListenerTouchOneByOne;

}

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

        static const std::string TOUCH_ENABLED_EVENT_NAME;

    public:

        static CellNode *create(size_t index, const cocos2d::Size &size, const std::function<void(size_t, Direction)> &onHit);

    public:

        bool init(const cocos2d::Size &size);

        void onExit() override;

        void setDirections(const std::unordered_map<Direction, bool> &directions);

        size_t getIndex() const;

        void setThingNode(ThingNode *thingNode);

        ThingNode *getThingNode() const;

        Thing getThing() const;

    private:

        static const float THING_SIZE_RATIO;

    private:

        const size_t mIndex;
        const std::function<void(size_t, Direction)> mOnHit;

        std::unordered_map<Direction, bool> mDirections;

        ThingNode *mThingNode = nullptr;

        bool mIsTouchesSkipped = false;

        cocos2d::EventListenerTouchOneByOne *mTouchListener;
        cocos2d::EventListenerCustom *mOnTouchEnabledListener;

    private:

        CellNode(size_t index, const std::function<void(size_t, Direction)> &onHit);

    private:

        bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

        void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

        void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

        void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

    };

}