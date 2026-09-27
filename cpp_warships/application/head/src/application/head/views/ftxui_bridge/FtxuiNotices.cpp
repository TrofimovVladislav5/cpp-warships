#include <application/head/views/ftxui_bridge/FtxuiNotices.h>

#include <string>
#include <utility>
#include <vector>

#include <application/head/views/Notices.h>
#include <application/head/views/ftxui_bridge/FtxuiPalette.h>

namespace cpp_warships::head {
    ftxui::Element noticeBlock(
            const Theme& theme,
            const model::ApplicationContext& application
    ) {
        const std::vector<std::string> notices = noticesToShow(application);
        if (notices.empty()) {
            return ftxui::emptyElement();
        }

        std::vector<ftxui::Element> lines;
        lines.reserve(notices.size());
        for (const std::string& notice : notices) {
            lines.push_back(ftxui::text(notice) | color(theme.danger));
        }

        return ftxui::vbox(std::move(lines));
    }
} // namespace cpp_warships::head
