//
// Created by ondesly on 2019-04-27.
//

#include <base/CCEventListenerTouch.h>
#include <base/CCEventListenerCustom.h>

#include "ThingNode.h"

#include "CellNode.h"

const std::string fm::CellNode::TOUCH_ENABLED_EVENT_NAME = "cell_node_touch_enabled";

const float fm::CellNode::THING_SIZE_RATIO = 0.75f;

fm::CellNode *fm::CellNode::create(const size_t index, const cocos2d::Size &size, const std::function<void(size_t, Direction)> &onHit) {
    auto node = new(std::nothrow) CellNode(index, onHit);
    if (node && node->init(size)) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

fm::CellNode::CellNode(const size_t index, const std::function<void(size_t, Direction)> &onHit)
        : mIndex(index), mOnHit(onHit) {

}

bool fm::CellNode::init(const cocos2d::Size &size) {
    if (!cocos2d::Node::init()) {
        return false;
    }

    setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    setContentSize(size);

    // Bg

    auto bg = cocos2d::Sprite::create();
    bg->setAnchorPoint(cocos2d::Vec2::ZERO);
    bg->setTextureRect(cocos2d::Rect(cocos2d::Vec2::ZERO, getContentSize()));
    addChild(bg);

    // Touch enabled listener

    mOnTouchEnabledListener = cocos2d::EventListenerCustom::create(TOUCH_ENABLED_EVENT_NAME, [&](cocos2d::EventCustom *event) {
        auto value = *static_cast<bool *>(event->getUserData());
        mTouchListener->setEnabled(value);
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mOnTouchEnabledListener, this);

    // Touch listener

    mTouchListener = cocos2d::EventListenerTouchOneByOne::create();
    mTouchListener->onTouchBegan = std::bind(&CellNode::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
    mTouchListener->onTouchMoved = std::bind(&CellNode::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
    mTouchListener->onTouchEnded = std::bind(&CellNode::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
    mTouchListener->onTouchCancelled = std::bind(&CellNode::onTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mTouchListener, this);

    return true;
}

void fm::CellNode::setDirections(const std::unordered_map<fm::CellNode::Direction, bool> &directions) {
    mDirections = directions;
}

size_t fm::CellNode::getIndex() const {
    return mIndex;
}

void fm::CellNode::setThingNode(ThingNode *const thingNode) {
    mThingNode = thingNode;

    if (mThingNode == nullptr) {
        return;
    }

    mThingNode->setScale(getContentSize().width / mThingNode->getContentSize().width * THING_SIZE_RATIO);
    mThingNode->setDefaultPosition(getPosition());
}

fm::ThingNode *fm::CellNode::getThingNode() const {
    return mThingNode;
}

fm::Thing fm::CellNode::getThing() const {
    if (mThingNode) {
        return mThingNode->getThing();
    } else {
        return {};
    }
}

bool fm::CellNode::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    if (isScreenPointInRect(touch->getLocation(), cocos2d::Camera::getVisitingCamera(), getWorldToNodeTransform(),
            cocos2d::Rect(cocos2d::Vec2::ZERO, getContentSize()), nullptr)) {
        mIsTouchesSkipped = false;
        return mThingNode != nullptr;
    } else {
        return false;
    }
}

void fm::CellNode::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    if (mIsTouchesSkipped) {
        return;
    }

    auto offset = touch->getLocation() - touch->getPreviousLocation();
    auto new_position = mThingNode->getPosition() + offset;
    auto deltaPosition = new_position - getPosition();
    auto deltaSize = getContentSize() / 2 - mThingNode->getContentSize() / 2 * mThingNode->getScale();

    // Check direction

    if (deltaPosition.x < 0 && !mDirections[Direction::LEFT]) {
        mThingNode->setPosition(getPosition());
        return;
    }

    if (deltaPosition.x > 0 && !mDirections[Direction::RIGHT]) {
        mThingNode->setPosition(getPosition());
        return;
    }

    if (deltaPosition.y > 0 && !mDirections[Direction::TOP]) {
        mThingNode->setPosition(getPosition());
        return;
    }

    if (deltaPosition.y < 0 && !mDirections[Direction::BOTTOM]) {
        mThingNode->setPosition(getPosition());
        return;
    }

    // One axis



    if (std::abs(deltaPosition.x) > std::abs(deltaPosition.y)) {
        new_position.y = getPosition().y;
        deltaPosition.y = 0;
    } else {
        new_position.x = getPosition().x;
        deltaPosition.x = 0;
    }

    //

    Direction direction = Direction::NONE;

    if (deltaPosition.x <= -deltaSize.width) {
        direction = Direction::LEFT;
        new_position.x = getPosition().x - deltaSize.width;
    } else if (deltaPosition.x >= deltaSize.width) {
        direction = Direction::RIGHT;
        new_position.x = getPosition().x + deltaSize.width;
    } else if (deltaPosition.y >= deltaSize.height) {
        direction = Direction::TOP;
        new_position.y = getPosition().y + deltaSize.height;
    } else if (deltaPosition.y <= -deltaSize.height) {
        direction = Direction::BOTTOM;
        new_position.y = getPosition().y - deltaSize.height;
    }

    mThingNode->setPosition(new_position);

    if (direction != Direction::NONE) {
        mIsTouchesSkipped = true;

        mThingNode->moveToDefaultPosition();
        mOnHit(mIndex, direction);
    }
}

void fm::CellNode::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    if (mIsTouchesSkipped) {
        return;
    }

    mThingNode->moveToDefaultPosition();
}

void fm::CellNode::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) {
    if (mIsTouchesSkipped) {
        return;
    }

    mThingNode->moveToDefaultPosition();
}

void fm::CellNode::onExit() {
    Node::onExit();

    _eventDispatcher->removeEventListener(mOnTouchEnabledListener);
    _eventDispatcher->removeEventListener(mTouchListener);
}
