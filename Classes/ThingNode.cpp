//
// Created by ondesly on 2019-04-28.
//

#include <base/CCEventListenerTouch.h>

#include "ThingNode.h"

fm::ThingNode *fm::ThingNode::create(const fm::Thing &thing, const std::function<void(ThingNode *)> &onPositionChanged) {
    auto node = new(std::nothrow) ThingNode(onPositionChanged);
    if (node && node->initWithSpriteFrameName(thing.name)) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

fm::ThingNode::ThingNode(const std::function<void(ThingNode *)> &onPositionChanged)
        : mOnPositionChanged(onPositionChanged) {

}

bool fm::ThingNode::initWithSpriteFrameName(const std::string &spriteFrameName) {
    if (!cocos2d::Sprite::initWithSpriteFrameName(spriteFrameName)) {
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

bool fm::ThingNode::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    mIsTouchPaused = false;
    cocos2d::Rect rect(cocos2d::Vec2::ZERO, getContentSize());
    return isScreenPointInRect(touch->getLocation(), cocos2d::Camera::getVisitingCamera(), getWorldToNodeTransform(), rect, nullptr);
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
    setPosition(getParent()->getContentSize() / 2);
}

void fm::ThingNode::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    setPosition(getParent()->getContentSize() / 2);
}

void fm::ThingNode::onExit() {
    Node::onExit();

    _eventDispatcher->removeEventListener(mTouchListener);
    mTouchListener->release();
}

void fm::ThingNode::setTouchPaused(const bool value) {
    mIsTouchPaused = value;
}
