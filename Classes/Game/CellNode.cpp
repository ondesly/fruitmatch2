//
// Created by ondesly on 2019-04-27.
//

#include "ThingNode.h"

#include "CellNode.h"

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

    auto bg = cocos2d::Sprite::create();
    bg->setAnchorPoint(cocos2d::Vec2::ZERO);
    bg->setTextureRect(cocos2d::Rect(cocos2d::Vec2::ZERO, getContentSize()));
    addChild(bg);

    return true;
}

size_t fm::CellNode::getIndex() const {
    return mIndex;
}

void fm::CellNode::setThingNode(ThingNode *const thingNode) {
    mThingNode = thingNode;

    if (mThingNode == nullptr) {
        return;
    }

    mThingNode->setOnPositionChanged(std::bind(&CellNode::onThingNodePositionChanged, this, std::placeholders::_1));
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

void fm::CellNode::onThingNodePositionChanged(ThingNode *const node) {
    auto deltaPosition = node->getPosition() - getPosition();
    auto deltaSize = getContentSize() / 2 - node->getContentSize() / 2 * node->getScale();

    Direction direction = Direction::NONE;
    bool isPaused = false;
    auto position = node->getPosition();

    if (deltaPosition.x <= -deltaSize.width) {
        direction = Direction::LEFT;
        isPaused = true;
        position.x = getPosition().x - deltaSize.width;
    } else if (deltaPosition.x >= deltaSize.width) {
        direction = Direction::RIGHT;
        position.x = getPosition().x + deltaSize.width;
        isPaused = true;
    }
    if (deltaPosition.y >= deltaSize.height) {
        direction = Direction::TOP;
        position.y = getPosition().y + deltaSize.height;
        isPaused = true;
    } else if (deltaPosition.y <= -deltaSize.height) {
        direction = Direction::BOTTOM;
        position.y = getPosition().y - deltaSize.height;
        isPaused = true;
    }

    node->setPosition(position);
    node->setTouchPaused(isPaused);

    if (direction != Direction::NONE) {
        node->moveToDefaultPosition();
        mOnHit(mIndex, direction);
    }
}