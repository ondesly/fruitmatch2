//
// Created by ondesly on 2019-04-28.
//

#pragma once

#include <2d/CCSprite.h>

#include "Thing.h"

namespace cocos2d {

    class EventListenerTouchOneByOne;

}

namespace fm {

    class ThingNode : public cocos2d::Sprite {
    public:

        static ThingNode *create(const Thing &thing, const std::function<void(ThingNode *)> &onPositionChanged);

    public:

        bool initWithSpriteFrameName(const std::string& spriteFrameName) override;

        void onExit() override;

        void setTouchPaused(bool value);

    private:

        const std::function<void(ThingNode *)> mOnPositionChanged;

        cocos2d::EventListenerTouchOneByOne *mTouchListener;
        bool mIsTouchPaused;

    private:

        ThingNode(const std::function<void(ThingNode *)> &onPositionChanged);

    private:

        bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

        void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

        void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

        void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

    };

}