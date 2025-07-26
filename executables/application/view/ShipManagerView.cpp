#include "ShipManagerView.h"

namespace cpp_warships::application {

    ShipManagerView::ShipManagerView(ShipManager& manager) : manager(manager) {}

    void ShipManagerView::displayShips() {
        for (int i = 0; i < manager.getShips().size(); i++) {
            std::cout << "Ship " << std::to_string(i + 1) << " length "
                      << manager.getShips()[i]->getLength() << " ";
            manager.getShips()[i]->status();
        }
    }
} // namespace cpp_warships::application