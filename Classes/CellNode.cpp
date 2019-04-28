//
// Created by ondesly on 2019-04-27.
//

#include "CellNode.h"

fm::CellNode *fm::CellNode::create(const cocos2d::Size &size) {
    auto cell = new(std::nothrow) CellNode();
    if (cell && cell->init(size)) {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return nullptr;
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