//
// Created by ondesly on 2019-04-28.
//

#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"
#include <base/CCEventListenerTouch.h>

#include "Constants.h"

#include "ThingNode.h"

const float fm::ThingNode::DISAPPEARANCE_SCALE = 0.01f;

const std::string fm::ThingNode::TOUCH_ENABLED_EVENT_NAME = "thing_node_touch_enabled";

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
    mOnTouchEnabled = cocos2d::EventListenerCustom::create(TOUCH_ENABLED_EVENT_NAME, [&](cocos2d::EventCustom *event) {
        auto value = *static_cast<bool *>(event->getUserData());
        mTouchListener->setEnabled(value);
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mOnTouchEnabled, this);
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

    // Touch

    mTouchListener = cocos2d::EventListenerTouchOneByOne::create();
    mTouchListener->retain();
    mTouchListener->setSwallowTouches(true);
    mTouchListener->onTouchBegan = std::bind(&ThingNode::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
    mTouchListener->onTouchMoved = std::bind(&ThingNode::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
    mTouchListener->onTouchEnded = std::bind(&ThingNode::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
    mTouchListener->onTouchCancelled = std::bind(&ThingNode::onTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mTouchListener, this);

    return true;
}

void fm::ThingNode::setDefaultPosition(const cocos2d::Vec2 &position) {
    mDefaultPosition = position;
}

void fm::ThingNode::setOnPositionChanged(const std::function<void(ThingNode *)> &onPositionChanged) {
    mOnPositionChanged = onPositionChanged;
}

bool fm::ThingNode::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    if (isScreenPointInRect(touch->getLocation(), cocos2d::Camera::getVisitingCamera(), getWorldToNodeTransform(),
            cocos2d::Rect(cocos2d::Vec2::ZERO, getContentSize()), nullptr)) {
        mIsTouchPaused = false;

        return true;
    } else {
        return false;
    }
}

void fm::ThingNode::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    if (mIsTouchPaused) {
        return;
    }

    auto offset = touch->getLocation() - touch->getPreviousLocation();
    setPosition(getPosition() + offset);

    mOnPositionChanged(this);
}

void fm::ThingNode::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    if (mIsTouchPaused) {
        return;
    }

    setPosition(mDefaultPosition);
}

void fm::ThingNode::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    if (mIsTouchPaused) {
        return;
    }

    setPosition(mDefaultPosition);
}

void fm::ThingNode::onExit() {
    Node::onExit();

    _eventDispatcher->removeEventListener(mOnTouchEnabled);

    _eventDispatcher->removeEventListener(mTouchListener);
    mTouchListener->release();
}

void fm::ThingNode::setTouchPaused(const bool value) {
    mIsTouchPaused = value;
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