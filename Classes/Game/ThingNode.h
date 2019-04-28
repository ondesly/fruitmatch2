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

        static const std::string TOUCH_ENABLED_EVENT_NAME;

    public:

        static ThingNode *create(const Thing &thing);

    public:

        bool init() override;

        void onExit() override;

        void setDefaultPosition(const cocos2d::Vec2 &position);

        void setOnPositionChanged(const std::function<void(ThingNode *)> &onPositionChanged);

        void setTouchPaused(bool value);

        const Thing &getThing() const;

        void moveToDefaultPosition(const std::function<void()> &onComplete = nullptr);

        void remove(const std::function<void()> &onComplete = nullptr);

        void show(const std::function<void()> &onComplete = nullptr);

    private:

        static const float DISAPPEARANCE_SCALE;

    private:

        const Thing mThing;

        std::function<void(ThingNode *)> mOnPositionChanged;

        cocos2d::EventListenerTouchOneByOne *mTouchListener;
        bool mIsTouchPaused;
        cocos2d::Vec2 mDefaultPosition;

        cocos2d::EventListenerCustom *mOnTouchEnabled;

    private:

        ThingNode(const Thing &thing);

    private:

        bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

        void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

        void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

        void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

    };

}