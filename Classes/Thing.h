//
// Created by ondesly on 2019-04-27.
//

#pragma once

#include <string>

namespace fm {

    struct Thing {

        std::string name;
        std::string color;

        bool operator==(const Thing &thing) const {
            return name == thing.name && color == thing.color;
        }

    };

}