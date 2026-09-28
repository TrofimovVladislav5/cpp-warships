#include <application/head/common/render/Notices.h>

#include <algorithm>
#include <cstddef>
#include <deque>

namespace cpp_warships::head {
    std::vector<std::string> noticesToShow(const model::ApplicationContext& application) {
        const std::deque<std::string>& all = application.notices();
        const auto shown = std::min(all.size(), static_cast<std::size_t>(NOTICES_SHOWN));

        return {all.end() - static_cast<std::ptrdiff_t>(shown), all.end()};
    }
}  // namespace cpp_warships::head
