#pragma once

#include <cpp_warships/input_parser/include/model/Parser.h>
#include <cpp_warships/input_parser/include/model/ParserCommandInfo.h>

#include "../GameState.h"

namespace cpp_warships::application {

    class TransitGameState final : public GameState {
    private:
        void handleExit(input_parser::ParsedOptions options);
        void handleMenu(input_parser::ParsedOptions options);
        std::string latestCommand;
        input_parser::SchemeMap<void> inputScheme;
    public:
        explicit TransitGameState(StateContext &context);
        void openState() override;
        void updateState() override;
        void closeState() override;
        GameState* transitToState() override;
    };
} // namespace cpp_warships::application
