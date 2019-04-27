//
// Created by ondesly on 2019-04-27.
//

#pragma once

#include <2d/CCScene.h>

namespace cocos2d {

    namespace ui {

        class Button;

        class Label;

        class Widget;

    }

}

namespace fm {

    class GameScene : public cocos2d::Scene {
    public:

        static GameScene *create(const std::string &level_name);

    public:

        bool init() override;

    private:

        const std::string m_level_name;

        cocos2d::Label *mMovesLabel;
        cocos2d::Label *mGoalLabel;

    private:

        GameScene(const std::string &level_name);

    private:

        cocos2d::ui::Widget *makeCell(bool is_enabled);

        cocos2d::ui::Button *makeBackButton();

    };

}