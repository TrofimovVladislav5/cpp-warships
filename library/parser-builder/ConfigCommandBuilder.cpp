#include "ConfigCommandBuilder.h"

#include "ParserCommandBuilder.h"

ConfigCommandBuilder& ConfigCommandBuilder::setDescription(std::string description) {
    this->description = std::move(description);
    return *this;
}

ConfigCommandBuilder& ConfigCommandBuilder::addParameter(ParserParameter parameter) {
    this->parameters.push_back(std::move(parameter));
    return *this;
}

ConfigCommandBuilder& ConfigCommandBuilder::setDisplayError(ParseCallback displayError) {
    this->displayError = std::move(displayError);
    return *this;
}

ConfigCommandBuilder& ConfigCommandBuilder::setCallback(ParseCallback function) {
    this->executable = std::move(function);
    return *this;
}

ConfigCommandBuilder& ConfigCommandBuilder::setResolveAllFlags(bool resolveAll) {
    this->resolveAllFlags = resolveAll;
    return *this;
}

ConfigCommandBuilder& ConfigCommandBuilder::setPrintHelp(ParseCallback help) {
    this->printHelp = help;
    return *this;
}


ParserCommandInfoConfig ConfigCommandBuilder::build() {
    return ParserCommandInfoConfig({
        description,
        parameters,
        executable ? executable : nullptr,
        displayError ? displayError : nullptr,
        resolveAllFlags ? resolveAllFlags : false,
        printHelp ? printHelp : nullptr
    });
}

ParserCommandInfoConfig ConfigCommandBuilder::buildAndReset() {
    ParserCommandInfoConfig config = build();
    this->reset();
    return config;
}