#pragma once

#include "GameField.h"

namespace cpp_warships::application {

    class PlayerFieldState {
    public:
        GameField* ownField;
        GameField* enemyField;
    };
} // namespace cpp_warships::application