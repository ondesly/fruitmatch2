//
// Created by ondesly on 2019-04-27.
//

#pragma once

#include <vector>

#include <math/CCGeometry.h>

#include "CellNode.h"
#include "TableLayout.h"
#include "Thing.h"

namespace fm {

    class GameLayout : public TableLayout {
    public:

        static GameLayout *create(int m, int n, float border);

    public:

        bool init() override;

        void addCell(bool is_enabled);

        void setThings(const std::vector<Thing> &things);

    private:

        cocos2d::Size mCellSize = cocos2d::Size::ZERO;

        std::vector<CellNode *> mCells;
        std::vector<Thing> mThings;

        std::vector<int> mOffset;
        std::uniform_int_distribution<size_t> mDistribution;

    private:

        GameLayout(int m, int n, float border);

    private:

        const fm::Thing &getRandomThing();

        bool isUniqueThing(size_t index, const Thing &thing);

        void onHit(size_t index, CellNode::Direction direction);

        size_t getNeighbourIndex(size_t index, CellNode::Direction direction) const;

        void swap(size_t indexFrom, size_t indexTo);

        void checkMatch();

    };

}