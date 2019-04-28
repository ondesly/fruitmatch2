//
// Created by ondesly on 2019-04-28.
//

#include <base/CCEventListenerTouch.h>

#include "ThingNode.h"

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
    setPosition(mDefaultPosition);
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
    setPosition(mDefaultPosition);
}

void fm::ThingNode::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    setPosition(mDefaultPosition);
}

void fm::ThingNode::onExit() {
    Node::onExit();

    _eventDispatcher->removeEventListener(mTouchListener);
    mTouchListener->release();
}

void fm::ThingNode::setTouchPaused(const bool value) {
    mIsTouchPaused = value;
}

const fm::Thing &fm::ThingNode::getThing() const {
    return mThing;
}
