//
// Created by ondesly on 2019-04-27.
//

#include <json/document.h>
#include <ui/UIButton.h>
#include <ui/UIImageView.h>
#include <ui/UIWidget.h>

#include "TableLayout.h"

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

    //

    rapidjson::Document document;

    auto levels_data = cocos2d::FileUtils::getInstance()->getStringFromFile("levels/level_" + m_level_name + ".json");
    document.Parse(levels_data.c_str());

    if (document.IsObject()) {
        auto width = document["width"].GetInt();
        auto field = document["field"].GetArray();

        auto table = TableLayout::create(width, field.Size() / width, 0.1f);
        table->setPosition(getContentSize() / 2);
        addChild(table);

        //

        for (auto it = field.Begin(); it != field.End(); ++it) {
            auto button = makeCell(it->GetInt() == 1);
            table->addChild(button);
        }
    }

    return true;
}

fm::GameScene::GameScene(const std::string &level_name) : m_level_name(level_name) {

}

cocos2d::ui::Widget *fm::GameScene::makeCell(bool is_enabled) {
    auto cell = cocos2d::ui::Widget::create();
    cell->setContentSize(cocos2d::Size(25, 25));

    auto bg = cocos2d::Sprite::create();
    bg->setAnchorPoint(cocos2d::Vec2::ZERO);
    bg->setTextureRect(cocos2d::Rect(cocos2d::Vec2::ZERO, cell->getContentSize()));
    bg->setVisible(is_enabled);
    cell->addChild(bg);

    return cell;
}
