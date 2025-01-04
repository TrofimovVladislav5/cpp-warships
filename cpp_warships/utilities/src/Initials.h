#pragma once

#ifdef WIN32
#include <windows.h>
#endif


namespace cpp_warships::utilities {
    class Initials {
    public:
        static void consoleOutInitials() {
            #ifdef WIN32
                system("chcp 65001");
                SetConsoleOutputCP(CP_UTF8);
            #endif
        }
    };
};
