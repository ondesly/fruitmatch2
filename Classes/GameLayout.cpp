//
// Created by ondesly on 2019-04-27.
//

#include <random>

#include "CellNode.h"
#include "ThingNode.h"

#include "GameLayout.h"

const std::string fm::GameLayout::ACTION_COMPLETE_EVENT_NAME = "game_layout_action_complete";
const std::string fm::GameLayout::SCORE_CHANGED_EVENT_NAME = "score_changed_action_complete";
const std::string fm::GameLayout::MOVES_CHANGED_EVENT_NAME = "moves_changed_action_complete";

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

    mOnActionComplete = cocos2d::EventListenerCustom::create(ACTION_COMPLETE_EVENT_NAME, [&](cocos2d::EventCustom *event) {
        auto action = *static_cast<Action *>(event->getUserData());
        switch (action) {
            case Action::SWAP:
                match();
                break;
            case Action::MATCH:
                fall();
                break;
            case Action::FALL:
                spawn();
                break;
            case Action::SPAWN:
                bool value = true;
                _eventDispatcher->dispatchCustomEvent(ThingNode::TOUCH_ENABLED_EVENT_NAME, &value);
                break;
        }

    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mOnActionComplete, this);

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

        auto thingNode = ThingNode::create(thing);
        thingNode->setPosition(cell->getPosition());
        addChild(thingNode);

        cell->setThingNode(thingNode);
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

            bool value = false;
            _eventDispatcher->dispatchCustomEvent(ThingNode::TOUCH_ENABLED_EVENT_NAME, &value);

            ++mMoves;
            _eventDispatcher->dispatchCustomEvent(MOVES_CHANGED_EVENT_NAME);
            break;
        }
    }
}

void fm::GameLayout::swap(const size_t indexFrom, const size_t indexTo) {
    auto cellFrom = mCells[indexFrom];
    auto thingNodeFrom = cellFrom->getThingNode();

    auto cellTo = mCells[indexTo];
    auto thingNodeTo = cellTo->getThingNode();

    cellFrom->setThingNode(thingNodeTo);
    thingNodeTo->moveToDefaultPosition();

    cellTo->setThingNode(thingNodeFrom);
    thingNodeFrom->moveToDefaultPosition([&]() {
        Action action(Action::SWAP);
        _eventDispatcher->dispatchCustomEvent(ACTION_COMPLETE_EVENT_NAME, &action);
    });
}

void fm::GameLayout::match() {
    std::vector<size_t> indexes;

    for (size_t i = 0; i < mCells.size(); ++i) {
        auto cell = mCells[i];

        if (!cell->isVisible() || cell->getThing().name.empty()) {
            continue;
        }

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

        auto thingNode = cell->getThingNode();
        if (index == indexes.front()) {
            thingNode->remove([&]() {
                Action action(Action::MATCH);
                _eventDispatcher->dispatchCustomEvent(ACTION_COMPLETE_EVENT_NAME, &action);
            });
        } else {
            thingNode->remove();
        }

        cell->setThingNode(nullptr);
    }

    mScore += indexes.size();

    _eventDispatcher->dispatchCustomEvent(SCORE_CHANGED_EVENT_NAME);
}

void fm::GameLayout::fall() {
    Action action(Action::FALL);
    _eventDispatcher->dispatchCustomEvent(ACTION_COMPLETE_EVENT_NAME, &action);
}

void fm::GameLayout::spawn() {
    Action action(Action::SPAWN);
    _eventDispatcher->dispatchCustomEvent(ACTION_COMPLETE_EVENT_NAME, &action);
}

size_t fm::GameLayout::getScore() const {
    return mScore;
}

size_t fm::GameLayout::getMoves() const {
    return mMoves;
}

void fm::GameLayout::onExit() {
    Node::onExit();

    _eventDispatcher->removeEventListener(mOnActionComplete);
}
