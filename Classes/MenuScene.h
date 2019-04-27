//
// Created by ondesly on 2019-04-26.
//

#pragma once

#include <2d/CCScene.h>

namespace cocos2d {

    namespace ui {

        class Button;

    }

}

namespace fm {

    class MenuScene : public cocos2d::Scene {
    public:

        static MenuScene *create();

    public:

        bool init() override;

    private:

        cocos2d::ui::Button *makeButton(const std::string &name);

    };

}