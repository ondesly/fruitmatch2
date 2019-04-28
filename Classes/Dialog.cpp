//
// Created by ondesly on 2019-04-28.
//

#include <ui/UIButton.h>

#include "Constants.h"

#include "Dialog.h"

fm::Dialog *fm::Dialog::create(cocos2d::Node *const parent, const std::string &text,
        const std::function<void(Dialog *)> &onOk, const std::function<void(Dialog *)> &onCancel) {
    auto dialog = new(std::nothrow) Dialog();
    if (dialog && dialog->init(parent, text, onOk, onCancel)) {
        dialog->autorelease();
        return dialog;
    }
    CC_SAFE_DELETE(dialog);
    return nullptr;
}

bool fm::Dialog::init(cocos2d::Node *const parent, const std::string &text, const std::function<void(Dialog *)> &onOk,
        const std::function<void(Dialog *)> &onCancel) {
    if (!cocos2d::Node::init()) {
        return false;
    }

    setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
    setContentSize(parent->getContentSize());

    //

    auto blackout = cocos2d::Sprite::create();
    blackout->setOpacity(150);
    blackout->setColor(Constants::BLACKOUT_COLOR);
    blackout->setAnchorPoint(cocos2d::Vec2::ZERO);
    blackout->setTextureRect(cocos2d::Rect(cocos2d::Vec2::ZERO, getContentSize()));
    addChild(blackout);

    //

    auto back = cocos2d::Sprite::create();
    back->setPosition(getContentSize() / 2);
    addChild(back);

    //

    auto ok = makeButton("OK", onOk);
    back->addChild(ok);

    auto label = cocos2d::Label::create();
    label->setSystemFontSize(cocos2d::Device::getDPI() * 0.15f);
    label->setColor(cocos2d::Color3B(0, 191, 124));
    label->setString(text);
    back->addChild(label);

    //

    auto border = ok->getContentSize().height / 2;

    cocos2d::Size size(
            ok->getContentSize().width * 2 + border * 3,
            ok->getContentSize().height + border * 5 + label->getContentSize().height);
    back->setTextureRect(cocos2d::Rect(cocos2d::Vec2::ZERO, size));

    //

    if (onCancel) {
        auto cancel = makeButton("Cancel", onCancel);
        back->addChild(cancel);

        ok->setPosition(cocos2d::Vec2(border, border) + ok->getContentSize() / 2);
        cancel->setPosition(cocos2d::Vec2(border * 2 + ok->getContentSize().width, border) + cancel->getContentSize() / 2);
    } else {
        ok->setPosition(cocos2d::Vec2(back->getContentSize().width / 2, ok->getContentSize().height / 2 + border));
    }

    label->setPosition(cocos2d::Vec2(
            back->getContentSize().width / 2,
            ok->getContentSize().height + border * 3 + label->getContentSize().height / 2));

    //

    return true;
}

cocos2d::ui::Button *fm::Dialog::makeButton(const std::string &text, const std::function<void(Dialog *)> &fn) {
    auto button = cocos2d::ui::Button::create("button", "", "", cocos2d::ui::Widget::TextureResType::PLIST);
    button->setScale9Enabled(true);

    auto size = button->getNormalTextureSize();
    button->setCapInsets(cocos2d::Rect(size.width * 1.75f, size.height * 1.75f, size.width * 1.75f, size.height * 1.75f));
    button->setZoomScale(-0.1f);

    button->setTitleFontSize(cocos2d::Device::getDPI() * 0.15f);
    button->setTitleText(text);

    button->setUnifySizeEnabled(true);
    button->setContentSize(button->getContentSize() + cocos2d::Size(button->getVirtualRendererSize().height * 2, 0.f));

    button->addTouchEventListener([&, fn](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            fn(this);
        }
    });

    return button;
}
