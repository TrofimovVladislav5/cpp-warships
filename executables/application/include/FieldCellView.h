#pragma once

#include <string>

namespace cpp_warships::application {

    class FieldCellView {
    private:
        bool top = false;
        bool right = false;
        bool bottom = false;
        bool left = false;
        std::string cellContent;

    public:
        explicit FieldCellView(std::string cellContent, bool top = false, bool right = false,
                               bool bottom = false, bool left = false);
        [[nodiscard]] std::string getTopEdge(bool start = true, bool end = true) const;
        [[nodiscard]] std::string getBottomEdge(bool start = true, bool end = true) const;
        [[nodiscard]] std::string getMiddleLayer(bool start = true, bool end = true) const;
        // [[nodiscard]] std::string getCellView() const;
        void printCellView() const;
    };
} // namespace cpp_warships::application