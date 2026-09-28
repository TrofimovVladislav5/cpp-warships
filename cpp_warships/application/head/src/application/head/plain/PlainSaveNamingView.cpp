#include <application/head/common/PresentationContext.h>
#include <application/head/common/render/Notices.h>
#include <application/head/plain/PlainFrame.h>
#include <application/head/plain/PlainSaveNamingView.h>

#include <string>
#include <vector>

namespace cpp_warships::head::plain {
    PlainSaveNamingView::PlainSaveNamingView(const common::PresentationContext& context) noexcept
        : context_(context) {}

    common::render::Frame PlainSaveNamingView::render(int, int) {
        const std::string typed = context_.state().naming.typedName;

        std::vector<std::string> lines{
            "==============================",
            "       NAME THIS MATCH        ",
            "==============================",
            "",
            "  name: " + typed + "_",
            ""
        };

        if (typed.empty()) {
            lines.emplace_back("  a name is needed before it can be put away");
            lines.emplace_back("");
        }

        lines.push_back(plainKeyLine("letters", "type a name"));
        lines.push_back(plainKeyLine("back", "rub one out"));
        if (!typed.empty()) {
            lines.push_back(plainKeyLine("enter", "save and quit"));
        }
        lines.push_back(plainKeyLine("esc", "back to the menu"));

        for (const std::string& notice : common::render::noticesToShow(context_.application())) {
            lines.push_back("  ! " + notice);
        }

        return common::render::frameOfLines(lines);
    }
}  // namespace cpp_warships::head::plain
