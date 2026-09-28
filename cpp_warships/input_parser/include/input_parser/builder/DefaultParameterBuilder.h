#pragma once
#include <input_parser/builder/ParserParameterBuilder.h>
#include <input_parser/model/ParserParameter.h>

namespace cpp_warships::input_parser::builder {
    class DefaultParameterBuilder : public ParserParameterBuilder {
    public:
        ~DefaultParameterBuilder() override = default;

        DefaultParameterBuilder& addFlag(std::string flag) override {
            this->flags.push_back(flag);
            return *this;
        };

        DefaultParameterBuilder& setValidator(std::regex validator) override {
            this->validator = validator;
            return *this;
        }

        DefaultParameterBuilder& setDescription(std::string description) override {
            this->description = description;
            return *this;
        }

        DefaultParameterBuilder& setNecessary(bool necessary) override {
            this->necessary = necessary;
            return *this;
        }

        model::ParserParameter build() {
            return model::ParserParameter({flags, validator, description, necessary});
        }

        model::ParserParameter buildAndReset() {
            model::ParserParameter parameter = this->build();
            this->reset();

            return parameter;
        }
    };
}  // namespace cpp_warships::input_parser::builder