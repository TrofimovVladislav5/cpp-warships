#pragma once

#include <input_parser/builder/ParserCommandBuilder.h>

#include <utility>

namespace cpp_warships::input_parser::builder {
    template <typename T>
    class CommandBuildDirector {
    private:
        ParserCommandBuilder<T>& commandBuilder;

    public:
        explicit CommandBuildDirector(ParserCommandBuilder<T>* commandBuilder)
            : commandBuilder(*commandBuilder) {}

        ParserCommandBuilder<T>& buildBasicCommand(
            model::ParseCallback<T> function,
            std::string description
        ) const {
            commandBuilder.setDescription(std::move(description));
            commandBuilder.setCallback(std::move(function));

            return commandBuilder;
        };

        void reset() const {
            commandBuilder.reset();
        }
    };
}  // namespace cpp_warships::input_parser::builder
