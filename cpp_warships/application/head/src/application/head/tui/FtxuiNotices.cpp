#include <application/head/common/Theme.h>
#include <application/head/common/render/Notices.h>
#include <application/head/tui/FtxuiNotices.h>
#include <application/head/tui/FtxuiPalette.h>
#include <application/model/ApplicationContext.h>

#include <string>
#include <utility>
#include <vector>

namespace cpp_warships::head::tui {
    ftxui::Element noticeBlock(
        const common::Theme& theme,
        const model::ApplicationContext& application
    ) {
        const std::vector<std::string> notices = common::render::noticesToShow(application);
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
}  // namespace cpp_warships::head::tui
