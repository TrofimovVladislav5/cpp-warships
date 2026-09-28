#include <application/head/common/render/Notices.h>
#include <application/head/ftxui/FtxuiNotices.h>
#include <application/head/ftxui/FtxuiPalette.h>

#include <string>
#include <utility>
#include <vector>

namespace cpp_warships::head {
    ftxui::Element noticeBlock(const Theme& theme, const model::ApplicationContext& application) {
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
}  // namespace cpp_warships::head
