#include "FieldCellView.h"

#include <cmath>
#include <iostream>

FieldCellView::FieldCellView(std::string cellContent, bool top, bool right, bool bottom, bool left)
    : top(top), right(right), bottom(bottom), left(left), cellContent(std::move(cellContent)) {}

std::string FieldCellView::getTopEdge(bool start, bool end) const {
    std::string result;
    if (top) {
        if (start) result += left ? "╔" : "╤";
        result += "═════";
        if (end) result += right ? "╗" : "╤";
    } else {
        if (start) result += left ? "╟" : "┼";
        result += "─────";
        if (end) result += right ? "╢" : "┼";
    }

    return result;
}

std::string FieldCellView::getBottomEdge(bool start, bool end) const {
    std::string result;
    if (bottom) {
        if (start) result += left ? "╚" : "╧";
        result += "═════";
        if (end) result += right ? "╝" : "╧";
    } else {
        if (start) result += left ? "╟" : "┼";
        result += "─────";
        if (end) result += right ? "╢" : "┼";
    }

    return result;
}

std::string FieldCellView::getMiddleLayer(bool start, bool end) const {
    std::string result;

    int spacesBefore = floor((5.0f - static_cast<float>(cellContent.size())) / 2.0f);
    int spacesAfter = ceil((5.0f - static_cast<float>(cellContent.size())) / 2.0f);

    if (start) result += left ? "║" : "│";
    result += std::string(spacesBefore, ' ') + cellContent + std::string(spacesAfter, ' ');
    if (end) result += right ? "║" : "│";

    return result;
}
