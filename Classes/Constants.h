//
// Created by ondesly on 2019-04-28.
//

#pragma once

#include <string>

#include <base/ccTypes.h>

namespace fm {

    class Constants {
    public:

        static const cocos2d::Color3B BG_COLOR;
        static const cocos2d::Color3B BUTTON_COLOR;

        static const cocos2d::Color3B BLACKOUT_COLOR;
        static const int BLACKOUT_OPACITY = 150;

    public:

        static const float BORDER_RATIO;

        static const float MEDIUM_TEXT_RATIO;
        static const float LARGE_TEXT_RATIO;

        static const float ANIMATION_DURATION;

    public:

        static const std::string BUTTON_TEXTURE_NAME;
        static const float BUTTON_INSET_RATIO;
        static const float BUTTON_ZOOM_SCALE;

    public:

        static const std::string TEXTURES_PATH;
        static const std::string TEXTURE_FILE_NAME;

        static const std::string PROGRESS_FILE_NAME;

        static const std::string LEVELS_PATH;
        static const std::string LEVELS_FILE_NAME;
        static const std::string LEVEL_FILE_PREFIX;
        static const std::string LEVEL_FILE_SUFFIX;

    };

}