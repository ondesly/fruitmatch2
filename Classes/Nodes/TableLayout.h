//
// Created by ondesly on 2019-04-27.
//

#pragma once

#include <2d/CCNode.h>

namespace fm {

    class TableLayout : public cocos2d::Node {
    public:

        static TableLayout *create(int m, int n, float border);

    public:

        bool init() override;

        virtual void layout();

    protected:

        const int mM;
        const int mN;

        const float mBorder;

    protected:

        TableLayout(int m, int n, float border);

    };

}