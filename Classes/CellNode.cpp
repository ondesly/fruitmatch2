//
// Created by ondesly on 2019-04-27.
//

#include "ThingNode.h"

#include "CellNode.h"

fm::CellNode *fm::CellNode::create(const size_t index, const cocos2d::Size &size) {
    auto node = new(std::nothrow) CellNode(index);
    if (node && node->init(size)) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

fm::CellNode::CellNode(const size_t index) : mIndex(index) {

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

void fm::CellNode::setThing(const Thing &thing) {
    mThing = thing;

    auto thingNode = ThingNode::create(thing, std::bind(&CellNode::onThingNodePositionChanged, this, std::placeholders::_1));
    thingNode->setScale(getContentSize().width / thingNode->getContentSize().width * 0.75f);
    thingNode->setPosition(getContentSize() / 2);
    addChild(thingNode);
}

const fm::Thing &fm::CellNode::getThing() const {
    return mThing;
}

void fm::CellNode::onThingNodePositionChanged(ThingNode *const node) {
    auto scaledHalfSize = node->getContentSize() / 2 * node->getScale();

    bool isPaused = false;
    auto position = node->getPosition();
    if (node->getPosition().x <= scaledHalfSize.width) {
        position.x = scaledHalfSize.width;
        isPaused = true;
    } else if (node->getPosition().x >= getContentSize().width - scaledHalfSize.width) {
        position.x = getContentSize().width - scaledHalfSize.width;
        isPaused = true;
    }
    if (node->getPosition().y >= getContentSize().height - scaledHalfSize.height) {
        position.y = getContentSize().height - scaledHalfSize.height;
        isPaused = true;
    } else if (node->getPosition().y <= scaledHalfSize.height) {
        position.y = scaledHalfSize.height;
        isPaused = true;
    }
    node->setPosition(position);
    node->setTouchPaused(isPaused);
}
