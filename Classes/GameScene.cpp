//
// Created by ondesly on 2019-04-27.
//

#include <unordered_map>

#include <base/CCEventListenerCustom.h>
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

    // Labels

    mScoreLabel = cocos2d::Label::create();
    mScoreLabel->setSystemFontSize(cocos2d::Device::getDPI() * 0.15f);
    mScoreLabel->setColor(cocos2d::Color3B(0, 191, 124));
    mScoreLabel->setString("Goal: 0/10");
    mScoreLabel->setPosition(cocos2d::Vec2(
            getContentSize().width - mScoreLabel->getContentSize().width / 2,
            getContentSize().height - mScoreLabel->getContentSize().height / 2));
    addChild(mScoreLabel);

    mMovesLabel = cocos2d::Label::create();
    mMovesLabel->setSystemFontSize(cocos2d::Device::getDPI() * 0.15f);
    mMovesLabel->setColor(cocos2d::Color3B(0, 191, 124));
    mMovesLabel->setString("Moves: 0");
    mMovesLabel->setPosition(cocos2d::Vec2(
            getContentSize().width / 2,
            getContentSize().height - mMovesLabel->getContentSize().height / 2));
    addChild(mMovesLabel);

    //

    rapidjson::Document document;

    auto levels_data = cocos2d::FileUtils::getInstance()->getStringFromFile("levels/level_" + mLevelName + ".json");
    document.Parse(levels_data.c_str());

    if (document.IsObject()) {
        auto width = document["width"].GetInt();
        auto field = document["field"].GetArray();

        mGame = GameLayout::create(width, field.Size() / width, 0.1f);
        mGame->setPosition(getContentSize() / 2);
        mGame->setContentSize(cocos2d::Size(
                getContentSize().width - backButton->getContentSize().width * 3,
                getContentSize().height - mMovesLabel->getContentSize().height * 3));
        addChild(mGame);

        //

        for (auto it = field.Begin(); it != field.End(); ++it) {
            mGame->addCell(it->GetInt() == 1);
        }
        mGame->layout();

        //

        std::vector<Thing> things;
        auto things_object = document["things"].GetObject();
        for (auto it = things_object.MemberBegin(); it != things_object.MemberEnd(); ++it) {
            auto name = it->name.GetString();
            auto color = it->value.GetString();
            things.push_back({name, color});
        }
        mGame->setThings(things);
    }

    // Listeners

    mOnScoreChanged = cocos2d::EventListenerCustom::create("score_changed", [&](cocos2d::EventCustom *event) {
        auto score = mGame->getScore();
        mScoreLabel->setString("Goal: " + std::to_string(score) + "/10");
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mOnScoreChanged, this);

    mOnMovesChanged = cocos2d::EventListenerCustom::create("moves_changed", [&](cocos2d::EventCustom *event) {
        auto moves = mGame->getMoves();
        mMovesLabel->setString("Moves: " + std::to_string(moves));
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mOnMovesChanged, this);

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

void fm::GameScene::onExit() {
    Node::onExit();

    _eventDispatcher->removeEventListener(mOnScoreChanged);
    _eventDispatcher->removeEventListener(mOnMovesChanged);
}
