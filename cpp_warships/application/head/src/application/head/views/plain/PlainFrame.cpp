#include <application/head/views/plain/PlainFrame.h>

#include <cstddef>

namespace cpp_warships::head {
    namespace {
        constexpr std::size_t KEY_COLUMN_WIDTH = 8;
    } // namespace

    std::string plainKeyLine(const std::string& key, const std::string& description) {
        const std::size_t padding =
                key.size() < KEY_COLUMN_WIDTH ? KEY_COLUMN_WIDTH - key.size() : 1;
        return "  " + key + std::string(padding, ' ') + description;
    }
} // namespace cpp_warships::head
