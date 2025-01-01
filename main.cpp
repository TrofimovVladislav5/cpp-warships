#include <src/library/defaults/Initials.h>

#include "GameController.h"

int main() {
    Initials::consoleOutInitials();

    GameController controller(new ConsoleInputReader());

    controller.run();

    return 0;
}
