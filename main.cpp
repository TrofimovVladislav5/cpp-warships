#include <GameController.h>
#include <library/defaults/Initials.h>

int main(){
    Initials::consoleOutInitials();
    GameController controller;
    controller.run();
    return 0;
}
