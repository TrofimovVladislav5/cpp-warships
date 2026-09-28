#include <application/head/tui/FtxuiPalette.h>
#include <application/head/tui/FtxuiView.h>

#include <ftxui/component/mouse.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/screen.hpp>
#include <map>
#include <utility>

namespace cpp_warships::head::tui {
    namespace {
        constexpr int DEFAULT_FRAME_WIDTH = 80;
        constexpr int DEFAULT_FRAME_HEIGHT = 24;

        [[nodiscard]] common::input::Key keyOf(const ftxui::Event& event) {
            static const std::map<ftxui::Event, common::input::Key> NAMED_KEYS = {
                {ftxui::Event::Return, common::input::Key::Enter},
                {ftxui::Event::Escape, common::input::Key::Escape},
                {ftxui::Event::Tab, common::input::Key::Tab},
                {ftxui::Event::Backspace, common::input::Key::Backspace},
                {ftxui::Event::Delete, common::input::Key::Delete},
                {ftxui::Event::ArrowUp, common::input::Key::ArrowUp},
                {ftxui::Event::ArrowDown, common::input::Key::ArrowDown},
                {ftxui::Event::ArrowLeft, common::input::Key::ArrowLeft},
                {ftxui::Event::ArrowRight, common::input::Key::ArrowRight},
                {ftxui::Event::PageUp, common::input::Key::PageUp},
                {ftxui::Event::PageDown, common::input::Key::PageDown},
            };

            const auto named = NAMED_KEYS.find(event);
            if (named != NAMED_KEYS.end()) {
                return named->second;
            }

            return event.is_character() ? common::input::Key::Character : common::input::Key::None;
        }

        [[nodiscard]] common::input::PointerButton buttonOf(const ftxui::Mouse& mouse) {
            static const std::map<ftxui::Mouse::Button, common::input::PointerButton> BUTTONS = {
                {ftxui::Mouse::Left, common::input::PointerButton::Left},
                {ftxui::Mouse::Right, common::input::PointerButton::Right},
                {ftxui::Mouse::Middle, common::input::PointerButton::Middle},
                {ftxui::Mouse::WheelUp, common::input::PointerButton::WheelUp},
                {ftxui::Mouse::WheelDown, common::input::PointerButton::WheelDown},
            };

            const auto button = BUTTONS.find(mouse.button);
            return button == BUTTONS.end() ? common::input::PointerButton::None : button->second;
        }

        /** @brief Paints a frame that was drawn elsewhere, taking up exactly
         * its own size. */
        class FrameNode final : public ftxui::Node {
        public:
            explicit FrameNode(common::render::Frame frame)
                : frame_(std::move(frame)) {}

            void ComputeRequirement() override {
                requirement_.min_x = frame_.width();
                requirement_.min_y = frame_.height();
            }

            void Render(ftxui::Screen& screen) override {
                for (int row = 0; row < frame_.height(); ++row) {
                    for (int column = 0; column < frame_.width(); ++column) {
                        paintCell(screen, column, row);
                    }
                }
            }

        private:
            void paintCell(ftxui::Screen& screen, const int column, const int row) const {
                const int screenX = box_.x_min + column;
                const int screenY = box_.y_min + row;
                if (screenX > box_.x_max || screenY > box_.y_max) {
                    return;
                }

                const common::render::FrameCell& cell = frame_.at(column, row);
                ftxui::Cell& target = screen.PixelAt(screenX, screenY);

                target.character = cell.glyph;
                target.bold = cell.isBold;
                if (cell.ink.has_value()) {
                    target.foreground_color = toFtxuiColor(*cell.ink);
                }
                if (cell.fill.has_value()) {
                    target.background_color = toFtxuiColor(*cell.fill);
                }
            }

            common::render::Frame frame_;
        };
    }  // namespace

    common::input::Keystroke keystrokeOf(ftxui::Event event) {
        if (event.is_mouse()) {
            const ftxui::Mouse& mouse = event.mouse();
            return common::input::Keystroke{
                .key = common::input::Key::Pointer,
                .button = buttonOf(mouse),
                .isPressed = mouse.motion == ftxui::Mouse::Pressed,
                .pointerX = mouse.x,
                .pointerY = mouse.y
            };
        }

        return common::input::Keystroke{
            .key = keyOf(event),
            .character = event.is_character() ? event.character() : std::string{}
        };
    }

    ftxui::Element elementOfFrame(const common::render::Frame& frame) {
        return std::make_shared<FrameNode>(frame);
    }

    common::render::Frame FtxuiRenderer::render(
        const int availableWidth,
        const int availableHeight
    ) {
        const int width = availableWidth > 0 ? availableWidth : DEFAULT_FRAME_WIDTH;
        const int height = availableHeight > 0 ? availableHeight : DEFAULT_FRAME_HEIGHT;

        ftxui::Element element = renderElement();
        ftxui::Screen screen =
            ftxui::Screen::Create(ftxui::Dimension::Fixed(width), ftxui::Dimension::Fixed(height));
        ftxui::Render(screen, element);

        common::render::Frame frame{width, height};
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                const ftxui::Cell& painted = screen.PixelAt(column, row);
                common::render::FrameCell& cell = frame.at(column, row);

                cell.glyph = painted.character.empty() ? " " : painted.character;
                cell.isBold = painted.bold;
                cell.ink = colorOf(painted.foreground_color);
                cell.fill = colorOf(painted.background_color);
            }
        }

        return frame;
    }
}  // namespace cpp_warships::head::tui
