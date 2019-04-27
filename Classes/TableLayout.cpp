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

fm::TableLayout::TableLayout(const int m, const int n, const float border) : m_m(m), m_n(n), m_border(border) {

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
            m_m * front->getContentSize().width * (1 + m_border),
            m_n * front->getContentSize().height * (1 + m_border)));

    auto children = getChildren();
    for (auto i = 0; i < children.size(); ++i) {
        auto child = children.at(i);
        child->setPosition(cocos2d::Vec2(
                ((i % m_m) + 0.5f) * child->getContentSize().width * (1 + m_border),
                (m_n - ((i / m_m) + 0.5f)) * child->getContentSize().height * (1 + m_border)));
    }
}

void fm::TableLayout::addChild(cocos2d::Node *child) {
    Node::addChild(child);
}

void fm::TableLayout::addChild(cocos2d::Node *child, int localZOrder) {
    Node::addChild(child, localZOrder);
}

void fm::TableLayout::addChild(cocos2d::Node *child, int localZOrder, int tag) {
    Node::addChild(child, localZOrder, tag);

    layout();
}

void fm::TableLayout::addChild(cocos2d::Node *child, int localZOrder, const std::string &name) {
    Node::addChild(child, localZOrder, name);

    layout();
}