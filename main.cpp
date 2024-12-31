#include <library/defaults/Initials.h>
#include "input-reader/InputReader.h"
#include "GameController.h"
#include "input-reader/command/CommandInputReader.h"

int main(){
    Initials::consoleOutInitials();
    InputReader<>* inputReader = new CommandInputReader("../input-command.txt");
    GameController<CommandInputReader> controller(inputReader);
    controller.run();

    return 0;
}
