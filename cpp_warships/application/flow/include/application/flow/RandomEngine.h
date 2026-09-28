#pragma once

#include <random>

namespace cpp_warships::flow {
    /** @brief The random source the whole flow layer draws from, passed by reference. */
    using RandomEngine = std::mt19937;

    /** @brief A randomly seeded engine, for when reproducibility is not required. */
    RandomEngine makeRandomlySeededEngine();
}  // namespace cpp_warships::flow
