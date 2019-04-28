//
// Created by ondesly on 2019-04-27.
//

#pragma once

#include <vector>

#include <math/CCGeometry.h>

#include "CellNode.h"
#include "TableLayout.h"
#include "Thing.h"

namespace cocos2d {

    class EventListenerCustom;

}

namespace fm {

    class GameLayout : public TableLayout {
    public:

        static const std::string NEXT_ACTION_EVENT_NAME;
        static const std::string SCORE_CHANGED_EVENT_NAME;
        static const std::string MOVES_CHANGED_EVENT_NAME;

    public:

        static GameLayout *create(int m, int n, float border);

    public:

        bool init() override;

        void onExit() override;

        void addCell(bool is_enabled);

        void setThings(const std::vector<Thing> &things);

        size_t getScore() const;

        size_t getMoves() const;

    private:

        cocos2d::Size mCellSize = cocos2d::Size::ZERO;

        std::vector<CellNode *> mCells;
        std::vector<Thing> mThings;

        std::vector<int> mOffset;
        std::uniform_int_distribution<size_t> mDistribution;

        size_t mScore = 0;
        size_t mMoves = 0;

        cocos2d::EventListenerCustom *mOnActionComplete;

    private:

        enum class Action {
            MATCH,
            FALL,
            SPAWN,
            DONE
        };

    private:

        GameLayout(int m, int n, float border);

    private:

        const fm::Thing &getRandomThing();

        bool isUniqueThing(size_t index, const Thing &thing);

        void onHit(size_t index, CellNode::Direction direction);

        size_t getNeighbourIndex(size_t index, CellNode::Direction direction) const;

        size_t getTopAvailableIndex(size_t index) const;

        size_t getFallAvailableIndex(size_t index) const;

        void swap(size_t indexFrom, size_t indexTo);

        void match();

        void fall();

        void spawn();

    };

}