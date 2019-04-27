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

        void addChild(Node *child) override;

        void addChild(Node *child, int localZOrder) override;

        void addChild(Node *child, int localZOrder, int tag) override;

        void addChild(Node *child, int localZOrder, const std::string &name) override;

    private:

        const int mM;
        const int mN;
        const float mBorder;

    private:

        TableLayout(int m, int n, float border);

    private:

        void layout();

    };

}