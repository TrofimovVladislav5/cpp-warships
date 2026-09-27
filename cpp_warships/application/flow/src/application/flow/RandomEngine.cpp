#include <application/flow/RandomEngine.h>

namespace cpp_warships::flow {

    RandomEngine makeRandomlySeededEngine() {
        std::random_device randomDevice;
        return RandomEngine{randomDevice()};
    }
} // namespace cpp_warships::flow
