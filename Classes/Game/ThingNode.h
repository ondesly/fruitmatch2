//
// Created by ondesly on 2019-04-28.
//

#pragma once

#include <2d/CCSprite.h>

#include "Thing.h"

namespace fm {

    class ThingNode : public cocos2d::Sprite {
    public:

        static ThingNode *create(const Thing &thing);

    public:

        bool init() override;

        void setDefaultPosition(const cocos2d::Vec2 &position);

        const Thing &getThing() const;

        void moveToDefaultPosition(const std::function<void()> &onComplete = nullptr);

        void remove(const std::function<void()> &onComplete = nullptr);

        void show(const std::function<void()> &onComplete = nullptr);

    private:

        static const float DISAPPEARANCE_SCALE;

    private:

        const Thing mThing;

        cocos2d::Vec2 mDefaultPosition;

    private:

        ThingNode(const Thing &thing);

    };

}