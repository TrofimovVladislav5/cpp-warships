#include <functional>

class TypesHelper {
public:
    template<class TClass, class TOptions>
    static std::function<void(TOptions)> methodToFunction(void (TClass::*method)(TOptions), TClass *instance) {
        return [method, instance](TOptions options) {
            (instance->*method)(options);
        };
    }
};
