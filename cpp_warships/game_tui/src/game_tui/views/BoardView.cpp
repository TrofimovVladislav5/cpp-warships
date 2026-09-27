#include <game_tui/views/BoardView.h>

#include <game_tui/views/CoordinateLabel.h>

#include <game_tui/views/ftxui_bridge/FtxuiPalette.h>

#include <algorithm>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include <game_tui/views/CellAppearance.h>

namespace cpp_warships::game_tui {
    namespace {
        constexpr int ROW_LABEL_WIDTH = 3;
        std::string rowLabel(int row) {
            const std::string number = std::to_string(row + 1);
            const int padding = ROW_LABEL_WIDTH - 1 - static_cast<int>(number.size());
            return std::string(static_cast<std::size_t>(std::max(padding, 0)), ' ') + number + " ";
        }

        /** @brief How many columns a whole grid of @p boardWidth cells takes up. */
        int gridWidthOf(int boardWidth) {
            return boardWidth * BOARD_COLUMN_PITCH - BOARD_TILE_GAP;
        }

        /** @brief The colours a cell is painted in, with the overlay having its say first. */
        CellColors colorsOf(
                const game_core::Board& board,
                game_core::Coordinate coordinate,
                game_core::Visibility visibility,
                const Theme& theme,
                const BoardOverlay& overlay
        ) {
            if (overlay.marked.contains(coordinate)) {
                return overlay.markColors;
            }
            if (overlay.cursor == coordinate) {
                return theme.cursor;
            }

            return appearanceOf(board.stateAt(coordinate, visibility), theme).colors;
        }

        std::string glyphOf(
                const game_core::Board& board,
                game_core::Coordinate coordinate,
                game_core::Visibility visibility,
                const Theme& theme,
                const BoardOverlay& overlay
        ) {
            if (overlay.marked.contains(coordinate)) {
                return " ";
            }

            return appearanceOf(board.stateAt(coordinate, visibility), theme).glyph;
        }

        ftxui::Element tileElement(const std::string& glyph, const CellColors& colors) {
            return ftxui::text(centredInTile(glyph)) | color(colors.ink) | bgcolor(colors.fill);
        }

        ftxui::Element spacer(int width, const Theme& theme) {
            return ftxui::text(std::string(static_cast<std::size_t>(width), ' ')) |
                   bgcolor(theme.background);
        }
    } // namespace

    ftxui::Element BoardView::render(
            const game_core::Board& board,
            game_core::Visibility visibility,
            const Theme& theme,
            const BoardOverlay& overlay
    ) {
        boardWidth_ = board.width();
        boardHeight_ = board.height();

        std::vector<ftxui::Element> columnHeaders;
        std::vector<ftxui::Element> rowHeaders;
        std::vector<ftxui::Element> gridRows;

        for (int column = 0; column < boardWidth_; ++column) {
            const bool isLast = column + 1 == boardWidth_;
            const auto trailing =
                    static_cast<std::size_t>(isLast ? 0 : BOARD_COLUMN_PITCH - BOARD_TILE_WIDTH);
            columnHeaders.push_back(
                    ftxui::text(centredInTile(columnLabel(column)) + std::string(trailing, ' ')) |
                    color(theme.textMuted)
            );
        }

        for (int row = 0; row < boardHeight_; ++row) {
            std::vector<ftxui::Element> cells;

            for (int column = 0; column < boardWidth_; ++column) {
                const game_core::Coordinate coordinate{column, row};
                cells.push_back(tileElement(
                        glyphOf(board, coordinate, visibility, theme, overlay),
                        colorsOf(board, coordinate, visibility, theme, overlay)
                ));

                if (column + 1 < boardWidth_) {
                    cells.push_back(spacer(BOARD_TILE_GAP, theme));
                }
            }

            rowHeaders.push_back(ftxui::text(rowLabel(row)) | color(theme.textMuted));
            gridRows.push_back(ftxui::hbox(std::move(cells)));

            if (row + 1 < boardHeight_) {
                rowHeaders.push_back(ftxui::text(std::string(ROW_LABEL_WIDTH, ' ')));
                gridRows.push_back(spacer(gridWidthOf(boardWidth_), theme));
            }
        }

        return ftxui::vbox(
                {ftxui::hbox(
                         {ftxui::text(std::string(ROW_LABEL_WIDTH, ' ')),
                          ftxui::hbox(std::move(columnHeaders))}
                 ),
                 ftxui::hbox(
                         {ftxui::vbox(std::move(rowHeaders)),
                          ftxui::vbox(std::move(gridRows)) | ftxui::reflect(gridBox_)}
                 )}
        );
    }

    std::optional<game_core::Coordinate> BoardView::cellAt(int screenX, int screenY) const {
        if (!gridBox_.Contain(screenX, screenY)) {
            return std::nullopt;
        }

        const game_core::Coordinate coordinate{
                (screenX - gridBox_.x_min) / BOARD_COLUMN_PITCH,
                (screenY - gridBox_.y_min) / BOARD_ROW_PITCH
        };

        if (coordinate.x >= boardWidth_ || coordinate.y >= boardHeight_) {
            return std::nullopt;
        }

        return coordinate;
    }
} // namespace cpp_warships::game_tui
