#pragma once

#include <application/head/common/Color.h>

#include <optional>
#include <string>
#include <vector>

namespace cpp_warships::head::common::render {
    /** @brief One drawn character and how it looks. */
    struct FrameCell {
        std::string glyph = " ";
        std::optional<Color> fill;
        std::optional<Color> ink;
        bool isBold = false;
    };

    /** @brief A finished picture of a screen: a grid of characters with their colours. */
    class Frame {
    public:
        Frame() = default;
        Frame(int width, int height);

        [[nodiscard]] int width() const noexcept;
        [[nodiscard]] int height() const noexcept;

        [[nodiscard]] const FrameCell& at(int column, int row) const;
        [[nodiscard]] FrameCell& at(int column, int row);

        /** @brief Writes @p text from @p column along, leaving colours as they were. */
        void write(int column, int row, const std::string& text);

    private:
        int width_ = 0;
        int height_ = 0;
        std::vector<FrameCell> cells_;
    };

    /** @brief A frame holding exactly @p lines, as wide as the longest of them. */
    [[nodiscard]] Frame frameOfLines(const std::vector<std::string>& lines);

    /** @brief @p frame written out for a terminal, with colour only where a cell asked for it. */
    [[nodiscard]] std::string frameToText(const Frame& frame);
}  // namespace cpp_warships::head::common::render
