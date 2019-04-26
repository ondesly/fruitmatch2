//
// Created by ondesly on 2019-04-26.
//

#include "MenuScene.h"

bool fm::MenuScene::init() {
    if (!cocos2d::Scene::init()) {
        return false;
    }

    auto bg = cocos2d::Sprite::create();
    bg->setAnchorPoint(cocos2d::Vec2::ZERO);
    bg->setTextureRect(cocos2d::Rect(cocos2d::Vec2::ZERO, getContentSize()));
    addChild(bg);

    return true;
}