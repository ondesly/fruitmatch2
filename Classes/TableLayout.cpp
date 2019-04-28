//
// Created by ondesly on 2019-04-27.
//

#include "TableLayout.h"

fm::TableLayout *fm::TableLayout::create(const int m, const int n, const float border) {
    auto layout = new(std::nothrow) TableLayout(m, n, border);
    if (layout && layout->init()) {
        layout->autorelease();
        return layout;
    }
    CC_SAFE_DELETE(layout);
    return nullptr;
}

fm::TableLayout::TableLayout(const int m, const int n, const float border) : mM(m), mN(n), mBorder(border) {

}

bool fm::TableLayout::init() {
    if (!cocos2d::Node::init()) {
        return false;
    }

    setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

    return true;
}

void fm::TableLayout::layout() {
    auto front = getChildren().front();
    setContentSize(cocos2d::Size(
            mM * front->getContentSize().width * (1 + mBorder),
            mN * front->getContentSize().height * (1 + mBorder)));

    auto children = getChildren();
    for (auto i = 0; i < children.size(); ++i) {
        auto child = children.at(i);
        child->setPosition(cocos2d::Vec2(
                ((i % mM) + 0.5f) * child->getContentSize().width * (1 + mBorder),
                (mN - ((i / mM) + 0.5f)) * child->getContentSize().height * (1 + mBorder)));
    }
}