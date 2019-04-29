//
// Created by ondesly on 2019-04-29.
//

#include <ui/UIButton.h>
#include <ui/UIScale9Sprite.h>

#include "Constants.h"

#include "UI.h"

float fm::UI::mBorder = 0;

float fm::UI::mFontMediumSize = 0;
float fm::UI::mFontLargeSize = 0;

float fm::UI::border() {
    if (mBorder == 0) {
        auto size = cocos2d::Director::getInstance()->getWinSize();
        mBorder = std::min(size.width, size.height) * Constants::BORDER_RATIO;
    }

    return mBorder;
}

float fm::UI::getFontMediumSize() {
    if (mFontMediumSize == 0) {
        auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(Constants::BUTTON_TEXTURE_NAME);
        mFontMediumSize = frame->getOriginalSize().height * Constants::MEDIUM_TEXT_RATIO;
    }

    return mFontMediumSize;
}

float fm::UI::getFontLargeSize() {
    if (mFontLargeSize == 0) {
        auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(Constants::BUTTON_TEXTURE_NAME);
        mFontLargeSize = frame->getOriginalSize().height * Constants::LARGE_TEXT_RATIO;
    }

    return mFontLargeSize;
}

cocos2d::ui::Button *fm::UI::makeButton() {
    auto button = cocos2d::ui::Button::create(Constants::BUTTON_TEXTURE_NAME, "", "",
            cocos2d::ui::Widget::TextureResType::PLIST);
    button->setScale9Enabled(true);
    button->getRendererNormal()->setColor(Constants::BUTTON_COLOR);

    const auto size = button->getNormalTextureSize();
    const float ratio = Constants::BUTTON_INSET_RATIO;
    button->setCapInsets(cocos2d::Rect(size.width * ratio, size.height * ratio, size.width * ratio, size.height * ratio));
    button->setZoomScale(Constants::BUTTON_ZOOM_SCALE);

    button->setTitleFontSize(getFontMediumSize());

    return button;
}
