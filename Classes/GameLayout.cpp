//
// Created by ondesly on 2019-04-27.
//

#include <random>

#include "CellNode.h"

#include "GameLayout.h"

fm::GameLayout *fm::GameLayout::create(int m, int n, float border) {
    auto layout = new(std::nothrow) GameLayout(m, n, border);
    if (layout && layout->init()) {
        layout->autorelease();
        return layout;
    }
    CC_SAFE_DELETE(layout);
    return nullptr;
}

bool fm::GameLayout::init() {
    if (!TableLayout::init()) {
        return false;
    }


    return true;
}

fm::GameLayout::GameLayout(int m, int n, float border) : TableLayout(m, n, border) {

}

void fm::GameLayout::addCell(bool is_enabled) {
    if (mCellSize.equals(cocos2d::Size::ZERO)) {
        auto size = std::min(getContentSize().width / (mM * (1 + mBorder)), getContentSize().height / (mN * (1 + mBorder)));
        mCellSize = cocos2d::Size(size, size);
    }

    auto cell = CellNode::create(mCellSize);
    cell->setVisible(is_enabled);
    addChild(cell);

    mCells.push_back(cell);
}

const fm::Thing &fm::GameLayout::getRandomThing() {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    static std::uniform_int_distribution<size_t> distribution(0, mThings.size() - 1);

    return mThings[distribution(rng)];
}

void fm::GameLayout::setThings(const std::vector<Thing> &things) {
    mThings = things;

    for (auto cell : mCells) {
        auto &thing = getRandomThing();
        if (cell->isVisible()) {
            cell->setThing(thing);
        }
    }
}