#pragma once
#include <input_parser/model/ParserCommandInfo.h>

namespace cpp_warships::input_parser::builder {
    template <typename T>
    class ParserCommandBuilder {
    protected:
        std::string description;
        std::vector<model::ParserParameter> parameters;
        model::ParseCallback<T> executable;
        model::ParseCallback<void> displayError;
        model::ParseCallback<void> printHelp;
        bool resolveAllFlags = false;

    public:
        virtual ~ParserCommandBuilder() = default;

        virtual ParserCommandBuilder& setDescription(std::string description) = 0;
        virtual ParserCommandBuilder& addParameter(model::ParserParameter parameter) = 0;
        virtual ParserCommandBuilder& setDisplayError(model::ParseCallback<void> displayError) = 0;
        virtual ParserCommandBuilder& setCallback(model::ParseCallback<T> function) = 0;
        virtual ParserCommandBuilder& setPrintHelp(model::ParseCallback<void> help) = 0;
        virtual ParserCommandBuilder& setResolveAllFlags(bool resolveAll) = 0;
        void reset() {
            description = "";
            parameters.clear();
            executable = nullptr;
            displayError = nullptr;
            printHelp = nullptr;
            setResolveAllFlags(false);
        }
    };
}  // namespace cpp_warships::input_parser::builder