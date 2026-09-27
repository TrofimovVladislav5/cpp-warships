#include <build_session.h>

namespace cpp_warships::application {
    head::Application buildSession(
            flow::RandomEngine& randomEngine,
            persistence::SaveArchive& saveArchive
    ) {
        return head::Application{randomEngine, saveArchive};
    }
} // namespace cpp_warships::application
