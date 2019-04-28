//
// Created by ondesly on 2019-04-27.
//

#include <unordered_map>

#include <json/document.h>
#include <ui/UIButton.h>
#include <ui/UIImageView.h>
#include <ui/UIWidget.h>

#include "GameLayout.h"
#include "MenuScene.h"
#include "Thing.h"

#include "GameScene.h"

fm::GameScene *fm::GameScene::create(const std::string &level_name) {
    auto scene = new(std::nothrow) GameScene(level_name);
    if (scene && scene->init()) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

bool fm::GameScene::init() {
    if (!cocos2d::Scene::init()) {
        return false;
    }

    //

    auto bg = cocos2d::Sprite::create();
    bg->setColor(cocos2d::Color3B(230, 230, 230));
    bg->setAnchorPoint(cocos2d::Vec2::ZERO);
    bg->setTextureRect(cocos2d::Rect(cocos2d::Vec2::ZERO, getContentSize()));
    addChild(bg);

    auto backButton = makeBackButton();
    backButton->setPosition(cocos2d::Vec2(
            backButton->getContentSize().width / 2,
            getContentSize().height - backButton->getContentSize().height / 2));
    addChild(backButton);

    mMovesLabel = cocos2d::Label::create();
    mMovesLabel->setSystemFontSize(cocos2d::Device::getDPI() * 0.15f);
    mMovesLabel->setColor(cocos2d::Color3B(0, 191, 124));
    mMovesLabel->setString("Moves left: 5");
    mMovesLabel->setPosition(cocos2d::Vec2(
            getContentSize().width / 2,
            getContentSize().height - mMovesLabel->getContentSize().height / 2));
    addChild(mMovesLabel);

    mGoalLabel = cocos2d::Label::create();
    mGoalLabel->setSystemFontSize(cocos2d::Device::getDPI() * 0.15f);
    mGoalLabel->setColor(cocos2d::Color3B(0, 191, 124));
    mGoalLabel->setString("Goal: 5/10");
    mGoalLabel->setPosition(cocos2d::Vec2(
            getContentSize().width - mGoalLabel->getContentSize().width / 2,
            getContentSize().height - mGoalLabel->getContentSize().height / 2));
    addChild(mGoalLabel);

    //

    rapidjson::Document document;

    auto levels_data = cocos2d::FileUtils::getInstance()->getStringFromFile("levels/level_" + mLevelName + ".json");
    document.Parse(levels_data.c_str());

    if (document.IsObject()) {
        auto width = document["width"].GetInt();
        auto field = document["field"].GetArray();

        auto game = GameLayout::create(width, field.Size() / width, 0.1f);
        game->setPosition(getContentSize() / 2);
        game->setContentSize(cocos2d::Size(
                getContentSize().width - backButton->getContentSize().width * 3,
                getContentSize().height - mMovesLabel->getContentSize().height * 3));
        addChild(game);

        //

        for (auto it = field.Begin(); it != field.End(); ++it) {
            game->addCell(it->GetInt() == 1);
        }
        game->layout();

        //

        std::vector<Thing> things;
        auto things_object = document["things"].GetObject();
        for (auto it = things_object.MemberBegin(); it != things_object.MemberEnd(); ++it) {
            auto name = it->name.GetString();
            auto color = it->value.GetString();
            things.push_back({name, color});
        }
        game->setThings(things);
    }

    return true;
}

fm::GameScene::GameScene(const std::string &level_name) : mLevelName(level_name) {

}

cocos2d::ui::Button *fm::GameScene::makeBackButton() {
    auto button = cocos2d::ui::Button::create("button", "", "", cocos2d::ui::Widget::TextureResType::PLIST);
    button->setScale9Enabled(true);

    auto size = button->getNormalTextureSize();
    button->setCapInsets(cocos2d::Rect(size.width * 1.75f, size.height * 1.75f, size.width * 1.75f, size.height * 1.75f));
    button->setZoomScale(-0.1f);

    button->setTitleFontSize(cocos2d::Device::getDPI() * 0.15f);
    button->setTitleText("Back");

    button->setUnifySizeEnabled(true);
    button->setContentSize(button->getVirtualRendererSize() + cocos2d::Size(button->getVirtualRendererSize().height * 0.75f, 0.f));

    button->addTouchEventListener([](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                auto scene = MenuScene::create();
                cocos2d::Director::getInstance()->replaceScene(scene);
                break;
            }
            default:
                break;
        }
    });

    return button;
}
