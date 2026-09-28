#pragma once

#include <application/model/MatchInPlay.h>
#include <application/persistence/SaveArchive.h>

namespace cpp_warships::model {
    /** @brief How an attempt to put a match away turned out. */
    enum class SaveOutcome {
        Saved,
        NoMatchInPlay,
        CouldNotWrite,
    };

    /** @brief Putting a match away and picking it back up again.
     *  One slot for now: several would want a screen to choose between them. */
    class SaveBehavior {
       public:
        /** @brief Saves @p inPlay into @p saveArchive, both of which must
         * outlive this. */
        SaveBehavior(MatchInPlay& inPlay, persistence::SaveArchive& saveArchive) noexcept;

        /** @brief Whether there is a saved match waiting to be picked back up.
         */
        [[nodiscard]] bool hasSavedMatch() const;

        /** @brief Writes the match in play over the save, and says how that
         * went.
         */
        [[nodiscard]] SaveOutcome saveMatch();

        /** @brief Replaces whatever is in play with the saved match.
         *  @return whether a save was there and could be read. */
        [[nodiscard]] bool loadMatch();

       private:
        MatchInPlay& inPlay_;
        persistence::SaveArchive& saveArchive_;
    };
}  // namespace cpp_warships::model
