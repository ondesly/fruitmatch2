//
// Created by ondesly on 2019-04-27.
//

#include "CellNode.h"

fm::CellNode *fm::CellNode::create(const size_t index, const cocos2d::Size &size) {
    auto cell = new(std::nothrow) CellNode(index);
    if (cell && cell->init(size)) {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
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

    auto thingSprite = cocos2d::Sprite::createWithSpriteFrameName(thing.name);
    thingSprite->setScale(getContentSize().width / thingSprite->getContentSize().width);
    thingSprite->setPosition(getContentSize() / 2);
    addChild(thingSprite);
}

const fm::Thing &fm::CellNode::getThing() const {
    return mThing;
}