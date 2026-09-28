#pragma once

#include <input_parser/model/ParserCommandInfo.h>
#include <input_parser/model/ParserParameter.h>
#include <utilities/ViewHelper.h>

#include <iostream>

namespace cpp_warships::input_parser {
    class DefaultHelp {
    public:
        static void PrintParam(const model::ParserParameter& param);

        template <typename T>
        static void PrintCommand(
            std::pair<std::string, model::ParserCommandInfo<T>> command,
            std::function<void(model::ParserParameter)> printParam
        ) {
            model::ParserCommandInfo currentCommand = command.second;
            ViewHelper::consoleOut("print '" + command.first + "': ", 1);
            ViewHelper::consoleOut("├── description: " + currentCommand.getDescription(), 1);

            std::vector<model::ParserParameter> params = currentCommand.getParams();
            if (params.empty()) {
                ViewHelper::consoleOut("└── params: empty", 1);
            } else {
                ViewHelper::consoleOut("└── params:", 1);
            }

            for (int i = 0; i < static_cast<int>(params.size()); i++) {
                ViewHelper::consoleOut("Param (" + std::to_string(i + 1) + ")", 2);
                printParam(params[i]);
            }
        }
    };
}  // namespace cpp_warships::input_parser