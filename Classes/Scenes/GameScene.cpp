//
// Created by ondesly on 2019-04-27.
//

#include <unordered_map>

#include <base/CCEventListenerCustom.h>
#include <json/document.h>
#include <json/stringbuffer.h>
#include <json/writer.h>
#include <math/Vec2.h>
#include <platform/CCFileUtils.h>
#include <ui/UIButton.h>
#include <ui/UIWidget.h>

#include "Constants.h"
#include "Dialog.h"
#include "GameLayout.h"
#include "MapScene.h"
#include "Thing.h"
#include "UI.h"

#include "GameScene.h"

const float fm::GameScene::CELL_BORDER = 0.1f;

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
    bg->setColor(Constants::BG_COLOR);
    bg->setAnchorPoint(cocos2d::Vec2::ZERO);
    bg->setTextureRect(cocos2d::Rect(cocos2d::Vec2::ZERO, getContentSize()));
    addChild(bg);

    //

    auto backButton = makeBackButton();
    backButton->setPosition(cocos2d::Vec2(
            backButton->getContentSize().width / 2 + UI::border(),
            getContentSize().height - backButton->getContentSize().height / 2 - UI::border()));
    addChild(backButton);

    // Score

    auto scoreTextLabel = makeLabel("Goal", UI::getFontMediumSize());
    scoreTextLabel->setPosition(cocos2d::Vec2(
            getContentSize().width - scoreTextLabel->getContentSize().width / 2 - UI::border() * 2,
            getContentSize().height * 0.6f + scoreTextLabel->getContentSize().height / 2));
    addChild(scoreTextLabel);

    mScoreLabel = makeLabel("00/00", UI::getFontLargeSize());
    mScoreLabel->setPosition(scoreTextLabel->getPosition() +
            cocos2d::Vec2(0.f, mScoreLabel->getContentSize().height / 2 + scoreTextLabel->getContentSize().height / 2));
    addChild(mScoreLabel);

    // Moves

    mMovesLabel = makeLabel("00", UI::getFontLargeSize());
    mMovesLabel->setPosition(cocos2d::Vec2(
            scoreTextLabel->getPosition().x,
            getContentSize().height * 0.4f - mMovesLabel->getContentSize().height / 2));
    addChild(mMovesLabel);

    auto movesTextLabel = makeLabel("Moves", UI::getFontMediumSize());
    movesTextLabel->setPosition(mMovesLabel->getPosition() +
            cocos2d::Vec2(0.f, -mMovesLabel->getContentSize().height / 2 - movesTextLabel->getContentSize().height / 2));
    addChild(movesTextLabel);

    //

    rapidjson::Document document;

    auto path = Constants::LEVELS_PATH + "/" + Constants::LEVEL_FILE_PREFIX + mLevelName + Constants::LEVEL_FILE_SUFFIX;
    auto levelsData = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
    document.Parse(levelsData.c_str());

    if (!document.HasParseError()) {
        mGoal = document["goal"].GetUint();
        mScoreLabel->setString("0/" + std::to_string(mGoal));

        mMoves = document["moves"].GetUint();
        mMovesLabel->setString(std::to_string(mMoves));

        auto width = document["width"].GetInt();
        auto field = document["field"].GetArray();

        mGame = GameLayout::create(width, field.Size() / width, CELL_BORDER);
        mGame->setPosition(getContentSize() / 2);
        mGame->setContentSize(cocos2d::Size(
                getContentSize().width - backButton->getContentSize().width * 2 - UI::border() * 4,
                getContentSize().height - UI::border() * 2));
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

    //

    setListeners();

    return true;
}

void fm::GameScene::setListeners() {
    mOnScoreChanged = cocos2d::EventListenerCustom::create(GameLayout::SCORE_CHANGED_EVENT_NAME,
            [&](cocos2d::EventCustom *event) {
                auto score = mGame->getScore();
                mScoreLabel->setString(std::to_string(score) + "/" + std::to_string(mGoal));
            });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mOnScoreChanged, this);

    mOnMovesChanged = cocos2d::EventListenerCustom::create(GameLayout::MOVES_CHANGED_EVENT_NAME,
            [&](cocos2d::EventCustom *event) {
                auto moves = mGame->getMoves();
                mMovesLabel->setString(std::to_string(mMoves - moves));
            });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mOnMovesChanged, this);

    mOnAction = cocos2d::EventListenerCustom::create(GameLayout::ACTION_EVENT_NAME, [&](cocos2d::EventCustom *event) {
        auto action = *static_cast<GameLayout::Action *>(event->getUserData());
        switch (action) {
            case GameLayout::Action::DONE: {
                if (mGame->getScore() >= mGoal) {
                    saveProgress();
                    showCompleteDialog(true);
                } else if (mMoves - mGame->getMoves() == 0) {
                    showCompleteDialog(false);
                }
                break;
            }
            default:
                break;
        }

    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mOnAction, this);
}

fm::GameScene::GameScene(const std::string &levelName) : mLevelName(levelName) {

}

cocos2d::ui::Button *fm::GameScene::makeBackButton() {
    auto button = UI::makeButton();
    button->setTitleText("Back");

    button->setUnifySizeEnabled(true);
    button->setContentSize(button->getContentSize() + cocos2d::Size(button->getVirtualRendererSize().height, 0.f));

    button->addTouchEventListener([&](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            auto dialog = Dialog::create(this, "Are you sure?",
                    [&](Dialog *const dialog) {
                        showMenu();
                    },
                    [](Dialog *const dialog) {
                        dialog->removeFromParent();
                    });
            addChild(dialog);
        }
    });

    return button;
}

cocos2d::Label *fm::GameScene::makeLabel(const std::string &text, float fontSize) const {
    auto label = cocos2d::Label::create();
    label->setSystemFontSize(fontSize);
    label->setColor(Constants::BUTTON_COLOR);
    label->setString(text);

    return label;
}

void fm::GameScene::onExit() {
    Node::onExit();

    _eventDispatcher->removeEventListener(mOnScoreChanged);
    _eventDispatcher->removeEventListener(mOnMovesChanged);
    _eventDispatcher->removeEventListener(mOnAction);
}

void fm::GameScene::showMenu() {
    auto scene = MapScene::create();
    cocos2d::Director::getInstance()->replaceScene(
            cocos2d::TransitionFade::create(Constants::ANIMATION_DURATION, scene, Constants::BG_COLOR));
}

void fm::GameScene::showCompleteDialog(bool success) {
    auto text = success ? "You won!" : "You lose!";
    auto dialog = Dialog::create(this, text,
            [&](Dialog *const dialog) {
                showMenu();
            });
    addChild(dialog);
}

void fm::GameScene::saveProgress() {
    rapidjson::Document document;

    auto path = cocos2d::FileUtils::getInstance()->getWritablePath() + Constants::PROGRESS_FILE_NAME;

    auto levelsData = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
    if (levelsData.empty()) {
        document.SetObject();
    } else {
        document.Parse(levelsData.c_str());
    }

    //

    document.AddMember(rapidjson::StringRef(mLevelName.c_str()), rapidjson::Value(true), document.GetAllocator());

    //

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    cocos2d::FileUtils::getInstance()->writeStringToFile(buffer.GetString(), path);
}
