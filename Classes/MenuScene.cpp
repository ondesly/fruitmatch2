//
// Created by ondesly on 2019-04-26.
//

#include <ui/UIButton.h>

#include "MenuScene.h"

bool fm::MenuScene::init() {
    if (!cocos2d::Scene::init()) {
        return false;
    }

    auto bg = cocos2d::Sprite::create();
    bg->setAnchorPoint(cocos2d::Vec2::ZERO);
    bg->setTextureRect(cocos2d::Rect(cocos2d::Vec2::ZERO, getContentSize()));
    addChild(bg);

    //

    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tex.plist");

    auto button = cocos2d::ui::Button::create("button", "", "", cocos2d::ui::Widget::TextureResType::PLIST);
    button->setScale9Enabled(true);

    auto size = button->getNormalTextureSize();
    button->setCapInsets(cocos2d::Rect(size.width * 1.75f, size.height * 1.75f, size.width * 1.75f, size.height * 1.75f));

    button->setContentSize(cocos2d::Size(100, 50));
    button->setPosition(cocos2d::Vec2(100, 100));
    button->setZoomScale(-0.1f);
    button->setTitleText("1");

    button->addTouchEventListener([&](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                break;
            default:
                break;
        }
    });

    addChild(button);

    return true;
}