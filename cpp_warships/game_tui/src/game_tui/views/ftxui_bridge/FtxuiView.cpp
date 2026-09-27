#include <game_tui/views/ftxui_bridge/FtxuiView.h>

#include <map>
#include <utility>

#include <ftxui/component/mouse.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/screen.hpp>

#include <game_tui/views/ftxui_bridge/FtxuiPalette.h>

namespace cpp_warships::game_tui {
    namespace {
        constexpr int DEFAULT_FRAME_WIDTH = 80;
        constexpr int DEFAULT_FRAME_HEIGHT = 24;

        [[nodiscard]] Key keyOf(const ftxui::Event& event) {
            static const std::map<ftxui::Event, Key> NAMED_KEYS = {
                    {ftxui::Event::Return, Key::Enter},
                    {ftxui::Event::Escape, Key::Escape},
                    {ftxui::Event::Tab, Key::Tab},
                    {ftxui::Event::Backspace, Key::Backspace},
                    {ftxui::Event::Delete, Key::Delete},
                    {ftxui::Event::ArrowUp, Key::ArrowUp},
                    {ftxui::Event::ArrowDown, Key::ArrowDown},
                    {ftxui::Event::ArrowLeft, Key::ArrowLeft},
                    {ftxui::Event::ArrowRight, Key::ArrowRight},
                    {ftxui::Event::PageUp, Key::PageUp},
                    {ftxui::Event::PageDown, Key::PageDown},
                    {ftxui::Event::F2, Key::SaveKey},
                    {ftxui::Event::F3, Key::LoadKey},
            };

            const auto named = NAMED_KEYS.find(event);
            if (named != NAMED_KEYS.end()) {
                return named->second;
            }

            return event.is_character() ? Key::Character : Key::None;
        }

        [[nodiscard]] PointerButton buttonOf(const ftxui::Mouse& mouse) {
            static const std::map<ftxui::Mouse::Button, PointerButton> BUTTONS = {
                    {ftxui::Mouse::Left, PointerButton::Left},
                    {ftxui::Mouse::Right, PointerButton::Right},
                    {ftxui::Mouse::Middle, PointerButton::Middle},
                    {ftxui::Mouse::WheelUp, PointerButton::WheelUp},
                    {ftxui::Mouse::WheelDown, PointerButton::WheelDown},
            };

            const auto button = BUTTONS.find(mouse.button);
            return button == BUTTONS.end() ? PointerButton::None : button->second;
        }

        /** @brief Paints a frame that was drawn elsewhere, taking up exactly its own size. */
        class FrameNode final : public ftxui::Node {
        public:
            explicit FrameNode(Frame frame)
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

                const FrameCell& cell = frame_.at(column, row);
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

            Frame frame_;
        };
    } // namespace

    Keystroke keystrokeOf(ftxui::Event event) {
        if (event.is_mouse()) {
            const ftxui::Mouse& mouse = event.mouse();
            return Keystroke{
                    .key = Key::Pointer,
                    .button = buttonOf(mouse),
                    .isPressed = mouse.motion == ftxui::Mouse::Pressed,
                    .pointerX = mouse.x,
                    .pointerY = mouse.y
            };
        }

        return Keystroke{
                .key = keyOf(event),
                .character = event.is_character() ? event.character() : std::string{}
        };
    }

    ftxui::Element elementOfFrame(const Frame& frame) {
        return std::make_shared<FrameNode>(frame);
    }

    Frame FtxuiView::render(const int availableWidth, const int availableHeight) {
        const int width = availableWidth > 0 ? availableWidth : DEFAULT_FRAME_WIDTH;
        const int height = availableHeight > 0 ? availableHeight : DEFAULT_FRAME_HEIGHT;

        ftxui::Element element = renderElement();
        ftxui::Screen screen = ftxui::Screen::Create(
                ftxui::Dimension::Fixed(width),
                ftxui::Dimension::Fixed(height)
        );
        ftxui::Render(screen, element);

        Frame frame{width, height};
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                const ftxui::Cell& painted = screen.PixelAt(column, row);
                FrameCell& cell = frame.at(column, row);

                cell.glyph = painted.character.empty() ? " " : painted.character;
                cell.isBold = painted.bold;
                cell.ink = colorOf(painted.foreground_color);
                cell.fill = colorOf(painted.background_color);
            }
        }

        return frame;
    }
} // namespace cpp_warships::game_tui
