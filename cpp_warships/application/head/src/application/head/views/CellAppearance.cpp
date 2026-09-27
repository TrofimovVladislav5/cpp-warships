#include <application/head/views/CellAppearance.h>

#include <cstddef>
#include <functional>
#include <unordered_map>

namespace cpp_warships::head {
    namespace {
        using AppearanceFactory = std::function<CellAppearance(const Theme&)>;

        const std::unordered_map<core::CellState, AppearanceFactory> APPEARANCE_BY_STATE{
                {core::CellState::Water,
                 [](const Theme& theme) {
                     return CellAppearance{"·", theme.water};
                 }},
                {core::CellState::Ship,
                 [](const Theme& theme) {
                     return CellAppearance{" ", theme.ship};
                 }},
                {core::CellState::Damaged,
                 [](const Theme& theme) {
                     return CellAppearance{"✳", theme.damaged};
                 }},
                {core::CellState::Destroyed,
                 [](const Theme& theme) {
                     return CellAppearance{"✖", theme.destroyed};
                 }},
                {core::CellState::Sunk,
                 [](const Theme& theme) {
                     return CellAppearance{"✖", theme.sunk};
                 }},
                {core::CellState::Miss, [](const Theme& theme) {
                     return CellAppearance{"◌", theme.miss};
                 }}
        };
    } // namespace

    CellAppearance appearanceOf(core::CellState state, const Theme& theme) {
        return APPEARANCE_BY_STATE.at(state)(theme);
    }

    std::string centredInTile(const std::string& glyph) {
        const auto leading = static_cast<std::size_t>((BOARD_TILE_WIDTH - 1) / 2);
        const auto trailing = static_cast<std::size_t>(BOARD_TILE_WIDTH - 1) - leading;
        return std::string(leading, ' ') + glyph + std::string(trailing, ' ');
    }

} // namespace cpp_warships::head
