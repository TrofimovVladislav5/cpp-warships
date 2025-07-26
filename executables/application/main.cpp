#include <cpp_warships/utilities/include/Initials.h>
#include <cpp_warships/input_reader/include/config_reader/ConfigInputReader.h>

#include "controller/GameController.h"

int main() {
    Initials::consoleOutInitials();

    const std::string filename = "../test.txt";
    GameController controller(new cpp_warships::input_reader::config_reader::ConfigInputReader(filename));

    controller.run();

    return 0;
}
