#pragma once
#include <functional>
#include <regex>

class TypesHelper {
public:
    template<class TClass>
    static std::function<void()> methodToFunction(void (TClass::*method)(), TClass *instance) {
        return [method, instance]() {
            (instance->*method)();
        };
    }

    template<class TClass, class TOptions>
    static std::function<void(TOptions)> methodToFunction(void (TClass::*method)(TOptions), TClass *instance) {
        return [method, instance](TOptions options) {
            (instance->*method)(options);
        };
    }

    static std::pair<int, int> cell(const std::string& coord);
};
