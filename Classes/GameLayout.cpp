//
// Created by ondesly on 2019-04-27.
//

#include <random>

#include "CellNode.h"

#include "GameLayout.h"

fm::GameLayout *fm::GameLayout::create(const int m, const int n, const float border) {
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

fm::GameLayout::GameLayout(const int m, const int n, const float border) : TableLayout(m, n, border) {

}

void fm::GameLayout::addCell(const bool is_enabled) {
    if (mCellSize.equals(cocos2d::Size::ZERO)) {
        const auto size = std::min(getContentSize().width / (mM * (1 + mBorder)), getContentSize().height / (mN * (1 + mBorder)));
        mCellSize = cocos2d::Size(size, size);
    }

    auto cell = CellNode::create(mCells.size(), mCellSize);
    cell->setVisible(is_enabled);
    addChild(cell);

    mCells.push_back(cell);
}

const fm::Thing &fm::GameLayout::getRandomThing() {
    static std::random_device rd;
    static std::mt19937 rng(rd());

    return mThings[mDistribution(rng)];
}

bool fm::GameLayout::isUniqueThing(const size_t index, const Thing &thing) {
    for (auto i = 0; i < 4; ++i) {
        const auto currentIndex = index + mOffset[i];

        if (currentIndex < 0 || currentIndex >= mCells.size()) {
            continue;
        }

        auto cell = mCells[currentIndex];
        if (cell->isVisible() && cell->getThing() == thing) {
            return false;
        }
    }

    return true;
}

void fm::GameLayout::setThings(const std::vector<Thing> &things) {
    mThings = things;

    mOffset = {-1, 1, -mM, mM};
    mDistribution = std::uniform_int_distribution<size_t>(0, mThings.size() - 1);

    for (auto cell : mCells) {
        if (!cell->isVisible()) {
            continue;
        }

        Thing thing;
        do {
            thing = getRandomThing();
        } while (!isUniqueThing(cell->getIndex(), thing));


        cell->setThing(thing);
    }
}