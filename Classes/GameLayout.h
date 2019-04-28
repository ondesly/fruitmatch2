//
// Created by ondesly on 2019-04-27.
//

#pragma once

#include <vector>

#include <math/CCGeometry.h>

#include "TableLayout.h"
#include "Thing.h"

namespace fm {

    class CellNode;

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

    private:

        GameLayout(int m, int n, float border);

    private:

        const fm::Thing &getRandomThing();

    };

}