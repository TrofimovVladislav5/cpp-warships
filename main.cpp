#include <library/defaults/Initials.h>

#include "GameController.h"
#include "input-reader/command/CommandInputReader.h"
#include "input-reader/config/ConfigInputReader.h"

int main(){
    Initials::consoleOutInitials();

    InputReader<>* inputReader = new CommandInputReader("../input-commands.txt");
    GameController controller(inputReader);
    controller.run();

    return 0;
}
