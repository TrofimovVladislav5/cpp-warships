#include <library/defaults/Initials.h>

#include "GameController.h"

int main(){
    Initials::consoleOutInitials();
    GameController controller;
    controller.run();

    return 0;
}
