//
// Created by ondesly on 2019-04-29.
//

#pragma once

namespace cocos2d {

    namespace ui {

        class Button;

    }

}

namespace fm {

    class UI {
    public:

        static float border();

        static float getFontMediumSize();

        static float getFontLargeSize();

        static cocos2d::ui::Button *makeButton();

    private:

        static float mBorder;

        static float mFontMediumSize;
        static float mFontLargeSize;

    };

}