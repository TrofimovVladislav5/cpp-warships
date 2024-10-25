#pragma once
#include <windows.h>


class Initials {
public:
    static void consoleOutInitials() {
        system("chcp 65001");
        SetConsoleOutputCP(CP_UTF8);
    }
};

