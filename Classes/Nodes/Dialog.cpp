//
// Created by ondesly on 2019-04-28.
//

#include <ui/UIButton.h>
#include <ui/UIWidget.h>

#include "Constants.h"
#include "UI.h"

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

    // Blackout

    auto blackout = cocos2d::ui::Widget::create();
    blackout->setContentSize(getContentSize());
    blackout->setPosition(getContentSize() / 2);
    blackout->setTouchEnabled(true);
    blackout->addTouchEventListener([&, onOk, onCancel](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            if (onCancel) {
                onCancel(this);
            } else {
                onOk(this);
            }
        }
    });
    addChild(blackout);

    auto blackoutBg = cocos2d::Sprite::create();
    blackoutBg->setOpacity(Constants::BLACKOUT_OPACITY);
    blackoutBg->setColor(Constants::BLACKOUT_COLOR);
    blackoutBg->setAnchorPoint(cocos2d::Vec2::ZERO);
    blackoutBg->setTextureRect(cocos2d::Rect(cocos2d::Vec2::ZERO, blackout->getContentSize()));
    blackout->addChild(blackoutBg);

    // Back

    auto back = cocos2d::Sprite::create();
    back->setPosition(getContentSize() / 2);
    addChild(back);

    //

    auto ok = makeButton("OK", onOk);
    back->addChild(ok);

    auto label = cocos2d::Label::create();
    label->setSystemFontSize(UI::getFontMediumSize());
    label->setColor(Constants::BUTTON_COLOR);
    label->setString(text);
    back->addChild(label);

    //

    cocos2d::Size size(
            ok->getContentSize().width * 2 + UI::border() * 3,
            ok->getContentSize().height + UI::border() * 5 + label->getContentSize().height);
    back->setTextureRect(cocos2d::Rect(cocos2d::Vec2::ZERO, size));

    //

    if (onCancel) {
        auto cancel = makeButton("Cancel", onCancel);
        back->addChild(cancel);

        ok->setPosition(cocos2d::Vec2(UI::border(), UI::border()) + ok->getContentSize() / 2);
        cancel->setPosition(cocos2d::Vec2(UI::border() * 2 + ok->getContentSize().width, UI::border()) + cancel->getContentSize() / 2);
    } else {
        ok->setPosition(cocos2d::Vec2(back->getContentSize().width / 2, ok->getContentSize().height / 2 + UI::border()));
    }

    label->setPosition(cocos2d::Vec2(
            back->getContentSize().width / 2,
            ok->getContentSize().height + UI::border() * 3 + label->getContentSize().height / 2));

    //

    return true;
}

cocos2d::ui::Button *fm::Dialog::makeButton(const std::string &text, const std::function<void(Dialog *)> &fn) {
    auto button = UI::makeButton();
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
