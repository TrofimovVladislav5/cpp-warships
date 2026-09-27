#include <application/head/intents/QuitIntent.h>

#include <application/head/host/Shell.h>
#include <application/head/intents/IntentContext.h>
#include <application/head/session/Application.h>

namespace cpp_warships::head {
    void QuitIntent::applyTo(const IntentContext& context) const {
        context.shell.requestQuit();
    }
} // namespace cpp_warships::head
