#include <application/core/Board.h>
#include <application/head/plain/PlainBoardGrid.h>

#include <cstddef>
#include <map>

namespace cpp_warships::head::plain {
    namespace {
        constexpr int CELL_INTERIOR_WIDTH = 3;
        constexpr char FIRST_COLUMN_LETTER = 'A';

        /** @brief The letter a cell is drawn as, as close to the old console game as the newer
         * states allow: a hull still holding is told apart from one finished off. */
        [[nodiscard]] char glyphOf(const core::CellState state) {
            static const std::map<core::CellState, char> GLYPHS = {
                {core::CellState::Water, ' '},
                {core::CellState::Ship, '#'},
                {core::CellState::Damaged, 'x'},
                {core::CellState::Destroyed, 'X'},
                {core::CellState::Sunk, '%'},
                {core::CellState::Miss, 'o'},
            };

            return GLYPHS.at(state);
        }

        /** @brief The blank space a row number is written into, so the grid
         * lines up under the column letters however many rows there are. */
        [[nodiscard]] std::size_t labelWidthFor(const int height) {
            return std::to_string(height).size() + 1;
        }

        [[nodiscard]] std::string ruleLine(const int width, const std::size_t labelWidth) {
            std::string line(labelWidth, ' ');
            line += '+';

            for (int column = 0; column < width; ++column) {
                line += std::string(CELL_INTERIOR_WIDTH, '-');
                line += '+';
            }

            return line;
        }

        [[nodiscard]] std::string columnLine(const int width, const std::size_t labelWidth) {
            std::string line(labelWidth, ' ');

            for (int column = 0; column < width; ++column) {
                line += "  ";
                line += static_cast<char>(FIRST_COLUMN_LETTER + column);
                line += ' ';
            }

            return line;
        }

        [[nodiscard]] std::string rowLabel(const int row, const std::size_t labelWidth) {
            const std::string number = std::to_string(row + 1);
            return std::string(labelWidth - 1 - number.size(), ' ') + number + ' ';
        }
    }  // namespace

    std::vector<std::string> plainBoardLines(
        const core::Board& board,
        const core::Visibility visibility,
        const PlainBoardOverlay& overlay
    ) {
        const int width = board.width();
        const int height = board.height();
        const std::size_t labelWidth = labelWidthFor(height);
        const std::string rule = ruleLine(width, labelWidth);

        std::vector<std::string> lines{columnLine(width, labelWidth), rule};

        for (int row = 0; row < height; ++row) {
            std::string line = rowLabel(row, labelWidth) + '|';

            for (int column = 0; column < width; ++column) {
                const core::Coordinate cell{column, row};
                const bool isMarked = overlay.marked.count(cell) > 0;
                const char glyph =
                    isMarked ? overlay.markGlyph : glyphOf(board.stateAt(cell, visibility));

                const bool isUnderCursor = overlay.cursor.has_value() && *overlay.cursor == cell;
                line += isUnderCursor ? '>' : ' ';
                line += glyph;
                line += isUnderCursor ? '<' : ' ';
                line += '|';
            }

            lines.push_back(line);
            lines.push_back(rule);
        }

        return lines;
    }

    std::string plainBoardLegend() {
        return "# ship   x hit   X destroyed   % sunk   o miss";
    }
}  // namespace cpp_warships::head::plain
