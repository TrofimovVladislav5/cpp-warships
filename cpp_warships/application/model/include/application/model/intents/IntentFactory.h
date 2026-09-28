#pragma once

#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/model/intents/GameIntent.h>

#include <optional>
#include <string>

namespace cpp_warships::model {
    class ApplicationContext;
}

namespace cpp_warships::model::intents {
    /** @brief Where intents come from, and the only thing that holds the whole
     * context. */
    class IntentFactory {
    public:
        /** @brief Builds against @p application, which must outlive every
         * intent it makes. */
        explicit IntentFactory(ApplicationContext& application) noexcept;

        [[nodiscard]] GameIntentPointer startMatch(int boardSize) const;
        [[nodiscard]] GameIntentPointer placeShip(
            core::Coordinate origin,
            core::Direction direction,
            int length
        ) const;
        [[nodiscard]] GameIntentPointer removeShip(core::Coordinate coordinate) const;
        [[nodiscard]] GameIntentPointer shuffleFleet() const;
        [[nodiscard]] GameIntentPointer beginBattle() const;
        [[nodiscard]] GameIntentPointer fireAt(core::Coordinate coordinate) const;
        [[nodiscard]] GameIntentPointer useSkill(std::optional<core::Coordinate> target) const;

        [[nodiscard]] GameIntentPointer saveMatch(std::string name) const;
        [[nodiscard]] GameIntentPointer loadMatch(std::string slot) const;
        [[nodiscard]] GameIntentPointer deleteSave(std::string slot) const;
        [[nodiscard]] GameIntentPointer finishSession() const;

    private:
        ApplicationContext& application_;
    };
}  // namespace cpp_warships::model::intents
