#pragma once

#include <application/head/common/input/InputKey.h>

#include <string>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::common::input::keys {
    /** @brief The save the player is looking at, or empty when there are none. */
    [[nodiscard]] std::string selectedSave(const PresentationContext& context);

    /** @brief Every binding here works from the context and nothing else. */
    class SaveBrowserKey : public InputKey {
    public:
        explicit SaveBrowserKey(PresentationContext& context) noexcept;

    protected:
        PresentationContext& context_;
    };

    /** @brief Moves the highlight up and down the list, stopping at either end. */
    class MoveSaveSelectionKey final : public SaveBrowserKey {
    public:
        using SaveBrowserKey::SaveBrowserKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Picks the highlighted save back up. */
    class LoadSelectedSaveKey final : public SaveBrowserKey {
    public:
        using SaveBrowserKey::SaveBrowserKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Throws the highlighted save away. */
    class DeleteSelectedSaveKey final : public SaveBrowserKey {
    public:
        using SaveBrowserKey::SaveBrowserKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };
}  // namespace cpp_warships::head::common::input::keys
