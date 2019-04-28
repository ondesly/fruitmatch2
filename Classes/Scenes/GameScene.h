//
// Created by ondesly on 2019-04-27.
//

#pragma once

#include <string>

#include <2d/CCScene.h>

namespace cocos2d {

    namespace ui {

        class Button;

        class Label;

    }

    class EventListenerCustom;

}

namespace fm {

    class GameLayout;

    class GameScene : public cocos2d::Scene {
    public:

        static GameScene *create(const std::string &level_name);

    public:

        bool init() override;

        void onExit() override;

    private:

        const std::string mLevelName;

        GameLayout *mGame;

        cocos2d::Label *mMovesLabel;
        cocos2d::Label *mScoreLabel;

        size_t mGoal = 0;
        size_t mMoves = 0;

        cocos2d::EventListenerCustom *mOnScoreChanged;
        cocos2d::EventListenerCustom *mOnMovesChanged;
        cocos2d::EventListenerCustom *mOnAction;

    private:

        GameScene(const std::string &level_name);

    private:

        cocos2d::ui::Button *makeBackButton();

        void setListeners();

        void showMenu();

    };

}