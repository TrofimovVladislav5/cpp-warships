#pragma once

#include <string>
#include <cpp_warships/input_parser/include/model/Parser.h>

#include "../GameState.h"
#include "../MatchBuilder.h"

namespace cpp_warships::application {

    class MenuGameState : public GameState{
    private:
        std::string latestCommand;
        input_parser::SchemeMap<void> inputScheme;
        MatchBuilder matchBuilder;
        bool isRunning;

        void handleGameLoad(input_parser::ParsedOptions options);
        void handleNewGame(input_parser::ParsedOptions options);
        void handleConfirm(input_parser::ParsedOptions options);
        void handleInfo(input_parser::ParsedOptions options);
        void handleList(input_parser::ParsedOptions options);
    public:
        explicit MenuGameState(StateContext& context);
        void openState() override;
        void closeState() override;
        void updateState() override;
        GameState* transitToState() override;
    };
} // namespace cpp_warships::application
