//
// Created by ondesly on 2019-04-26.
//

#include <json/document.h>
#include <ui/UIButton.h>
#include <ui/UIScale9Sprite.h>

#include "Constants.h"
#include "GameScene.h"
#include "TableLayout.h"

#include "MenuScene.h"

fm::MenuScene *fm::MenuScene::create() {
    auto scene = new(std::nothrow) MenuScene();
    if (scene && scene->init()) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

bool fm::MenuScene::init() {
    if (!cocos2d::Scene::init()) {
        return false;
    }

    //

    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tex.plist");

    //

    auto bg = cocos2d::Sprite::create();
    bg->setColor(Constants::BG_COLOR);
    bg->setAnchorPoint(cocos2d::Vec2::ZERO);
    bg->setTextureRect(cocos2d::Rect(cocos2d::Vec2::ZERO, getContentSize()));
    addChild(bg);

    //

    rapidjson::Document progressDoc;
    auto path = cocos2d::FileUtils::getInstance()->getWritablePath() + Constants::PROGRESS_FILE_NAME;
    auto progressData = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
    if (progressData.empty()) {
        progressDoc.SetObject();
    } else {
        progressDoc.Parse(progressData.c_str());
        if (progressDoc.HasParseError()) {
            progressDoc.SetObject();
        }
    }

    //

    rapidjson::Document levelsDoc;
    auto levelsData = cocos2d::FileUtils::getInstance()->getStringFromFile("levels/levels.json");
    levelsDoc.Parse(levelsData.c_str());

    if (!levelsDoc.HasParseError()) {
        auto m = levelsDoc["m"].GetInt();
        auto n = levelsDoc["n"].GetInt();
        auto b = levelsDoc["b"].GetFloat();

        auto table = TableLayout::create(m, n, b);
        table->setPosition(getContentSize() / 2);
        addChild(table);

        //

        auto levels = levelsDoc["levels"].GetArray();
        for (auto it = levels.Begin(); it != levels.End(); ++it) {
            cocos2d::ui::Button *button;
            if (progressDoc.HasMember(it->GetString())) {
                button = makeButton(it->GetString(), "✓");
            } else {
                button = makeButton(it->GetString(), it->GetString());
            }
            table->addChild(button);
        }
        table->layout();
    }

    return true;
}

cocos2d::ui::Button *fm::MenuScene::makeButton(const std::string &name, const std::string &title) {
    auto button = cocos2d::ui::Button::create("button", "", "", cocos2d::ui::Widget::TextureResType::PLIST);
    button->setScale9Enabled(true);
    button->getRendererNormal()->setColor(Constants::BUTTON_COLOR);

    const auto size = button->getNormalTextureSize();
    button->setCapInsets(cocos2d::Rect(size.width * 1.75f, size.height * 1.75f, size.width * 1.75f, size.height * 1.75f));
    button->setZoomScale(-0.1f);

    button->setTitleFontSize(cocos2d::Device::getDPI() * 0.2f);
    button->setTitleText(title);

    button->setUnifySizeEnabled(true);
    button->setContentSize(button->getVirtualRendererSize() + button->getVirtualRendererSize() * 0.75f);

    button->addTouchEventListener([name](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            auto scene = GameScene::create(name);
            cocos2d::Director::getInstance()->replaceScene(
                    cocos2d::TransitionFade::create(Constants::ANIMATION_DURATION, scene, Constants::BG_COLOR));
        }
    });

    return button;
}
