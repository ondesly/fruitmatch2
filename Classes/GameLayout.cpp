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

    auto cell = CellNode::create(mCells.size(), mCellSize,
            std::bind(&GameLayout::onHit, this, std::placeholders::_1, std::placeholders::_2));
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

size_t fm::GameLayout::getNeighbourIndex(size_t index, const fm::CellNode::Direction direction) const {
    switch (direction) {
        case CellNode::Direction::LEFT:
            index -= 1;
            break;
        case CellNode::Direction::TOP:
            index -= mM;
            break;
        case CellNode::Direction::RIGHT:
            index += 1;
            break;
        case CellNode::Direction::BOTTOM:
            index += mM;
            break;
        default:
            break;
    }

    if (index < 0 || index >= mCells.size() || !mCells[index]->isVisible()) {
        return mCells.size();
    } else {
        return index;
    }
}

void fm::GameLayout::onHit(const size_t index, const fm::CellNode::Direction direction) {

    // Neighbour

    const auto neighbourIndex = getNeighbourIndex(index, direction);

    if (neighbourIndex == mCells.size()) {
        return;
    }

    // Neighbours of neighbour

    size_t neighbours[4] = {
            getNeighbourIndex(neighbourIndex, CellNode::Direction::LEFT),
            getNeighbourIndex(neighbourIndex, CellNode::Direction::TOP),
            getNeighbourIndex(neighbourIndex, CellNode::Direction::RIGHT),
            getNeighbourIndex(neighbourIndex, CellNode::Direction::BOTTOM)
    };

    auto cell = mCells[index];
    for (auto i : neighbours) {
        if (i == mCells.size() || i == index) {
            continue;
        }

        auto neighbour = mCells[i];
        if (neighbour->getThing() == cell->getThing()) {
            swap(index, neighbourIndex);
            checkMatch();
            break;
        }
    }
}

void fm::GameLayout::swap(const size_t indexFrom, const size_t indexTo) {
    auto cellFrom = mCells[indexFrom];
    auto thingFrom = cellFrom->getThing();

    auto cellTo = mCells[indexTo];
    cellFrom->setThing(cellTo->getThing());
    cellTo->setThing(thingFrom);

    ++mMoves;

    _eventDispatcher->dispatchCustomEvent("moves_changed");
}

void fm::GameLayout::checkMatch() {
    std::vector<size_t> indexes;

    for (size_t i = 0; i < mCells.size(); ++i) {
        auto cell = mCells[i];

        for (auto j = 0; j < 4; ++j) {
            const auto neighbourIndex = i + mOffset[j];

            if (neighbourIndex < 0 || neighbourIndex >= mCells.size()) {
                continue;
            }

            auto neighbour = mCells[neighbourIndex];
            if (neighbour->isVisible() && cell->getThing() == neighbour->getThing()) {
                indexes.push_back(i);
                break;
            }
        }
    }

    for (auto index : indexes) {
        auto cell = mCells[index];
        cell->setThing({});
    }

    mScore += indexes.size();

    _eventDispatcher->dispatchCustomEvent("score_changed");
}

size_t fm::GameLayout::getScore() const {
    return mScore;
}

size_t fm::GameLayout::getMoves() const {
    return mMoves;
}
