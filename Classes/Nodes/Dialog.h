//
// Created by ondesly on 2019-04-28.
//

#pragma once

#include <functional>
#include <string>

#include <2d/CCNode.h>

namespace cocos2d {

    namespace ui {

        class Button;

    }

}

namespace fm {

    class Dialog : public cocos2d::Node {
    public:

        static Dialog *create(cocos2d::Node *parent, const std::string &text, const std::function<void(Dialog *)> &onOk,
                const std::function<void(Dialog *)> &onCancel = nullptr);

    public:

        bool init(cocos2d::Node *parent, const std::string &text, const std::function<void(Dialog *)> &onOk,
                const std::function<void(Dialog *)> &onCancel);

    private:

        cocos2d::ui::Button *makeButton(const std::string &text, const std::function<void(Dialog *)> &fn);

    };

}