#pragma once

#include <string>
#include <vector>

namespace cpp_warships::head {
    /** @brief One entry of a key list, the key padded so the descriptions line
     * up.
     */
    [[nodiscard]] std::string plainKeyLine(const std::string& key, const std::string& description);
}  // namespace cpp_warships::head
