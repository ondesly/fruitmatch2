//
// Created by ondesly on 2019-04-26.
//

#include <json/document.h>
#include <ui/UIButton.h>

#include "Constants.h"
#include "GameScene.h"
#include "TableLayout.h"
#include "UI.h"

#include "MapScene.h"

fm::MapScene *fm::MapScene::create() {
    auto scene = new(std::nothrow) MapScene();
    if (scene && scene->init()) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

bool fm::MapScene::init() {
    if (!cocos2d::Scene::init()) {
        return false;
    }

    //

    auto bg = cocos2d::Sprite::create();
    bg->setColor(Constants::BG_COLOR);
    bg->setAnchorPoint(cocos2d::Vec2::ZERO);
    bg->setTextureRect(cocos2d::Rect(cocos2d::Vec2::ZERO, getContentSize()));
    addChild(bg);

    //

    auto path = cocos2d::FileUtils::getInstance()->getWritablePath() + Constants::PROGRESS_FILE_NAME;
    auto progressData = cocos2d::FileUtils::getInstance()->getStringFromFile(path);

    rapidjson::Document progressDoc;
    if (progressData.empty()) {
        progressDoc.SetObject();
    } else {
        progressDoc.Parse(progressData.c_str());
        if (progressDoc.HasParseError()) {
            progressDoc.SetObject();
        }
    }

    //

    path = Constants::LEVELS_PATH + "/" + Constants::LEVELS_FILE_NAME;
    auto levelsData = cocos2d::FileUtils::getInstance()->getStringFromFile(path);

    rapidjson::Document levelsDoc;
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

cocos2d::ui::Button *fm::MapScene::makeButton(const std::string &name, const std::string &title) {
    auto button = UI::makeButton();
    button->setTitleText(title);

    button->setUnifySizeEnabled(true);
    button->setContentSize(button->getVirtualRendererSize() + button->getVirtualRendererSize() * 0.5f);

    button->addTouchEventListener([name](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            auto scene = GameScene::create(name);
            cocos2d::Director::getInstance()->replaceScene(
                    cocos2d::TransitionFade::create(Constants::ANIMATION_DURATION, scene, Constants::BG_COLOR));
        }
    });

    return button;
}
