//
// Created by ondesly on 2019-04-28.
//

#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"

#include "Constants.h"

#include "ThingNode.h"

const float fm::ThingNode::DISAPPEARANCE_SCALE = 0.01f;

fm::ThingNode *fm::ThingNode::create(const fm::Thing &thing) {
    auto node = new(std::nothrow) ThingNode(thing);
    if (node && node->init()) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

fm::ThingNode::ThingNode(const Thing &thing) : mThing(thing) {

}

bool fm::ThingNode::init() {
    if (!cocos2d::Sprite::initWithSpriteFrameName(mThing.name)) {
        return false;
    }

    // Color

    int color = 0;
    std::stringstream ss;
    ss << std::hex << mThing.color;
    ss >> color;

    setColor(cocos2d::Color3B(
            GLubyte((color >> 16) & 255),
            GLubyte((color >> 8) & 255),
            GLubyte(color & 255)));

    return true;
}

void fm::ThingNode::setDefaultPosition(const cocos2d::Vec2 &position) {
    mDefaultPosition = position;
}

const fm::Thing &fm::ThingNode::getThing() const {
    return mThing;
}

void fm::ThingNode::moveToDefaultPosition(const std::function<void()> &onComplete) {
    stopAllActions();
    runAction(cocos2d::Sequence::create(
            cocos2d::MoveTo::create(Constants::ANIMATION_DURATION, mDefaultPosition),
            cocos2d::CallFunc::create(onComplete),
            nullptr
    ));
}

void fm::ThingNode::remove(const std::function<void()> &onComplete) {
    stopAllActions();
    runAction(cocos2d::Sequence::create(
            cocos2d::ScaleTo::create(Constants::ANIMATION_DURATION, DISAPPEARANCE_SCALE),
            cocos2d::CallFunc::create(onComplete),
            cocos2d::CallFunc::create([&]() {
                removeFromParent();
            }),
            nullptr
    ));
}

void fm::ThingNode::show(const std::function<void()> &onComplete) {
    stopAllActions();
    runAction(cocos2d::Sequence::create(
            cocos2d::ScaleTo::create(Constants::ANIMATION_DURATION, getScale()),
            cocos2d::CallFunc::create(onComplete),
            nullptr
    ));
    setScale(DISAPPEARANCE_SCALE);
}