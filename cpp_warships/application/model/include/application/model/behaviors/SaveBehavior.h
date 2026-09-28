#pragma once

#include <application/persistence/SaveSummary.h>

#include <string>
#include <vector>

namespace cpp_warships::model {
    class MatchInPlay;
}

namespace cpp_warships::persistence {
    class SaveArchive;
}

namespace cpp_warships::model::behaviors {
    /** @brief How an attempt to put a match away turned out. */
    enum class SaveOutcome {
        Saved,
        NoMatchInPlay,
        CouldNotWrite,
    };

    /** @brief Putting matches away and picking them back up. There are as many saves as
     * the player cares to keep, each named for the moment it was made. */
    class SaveBehavior {
    public:
        /** @brief Saves @p inPlay into @p saveArchive, both of which must outlive this. */
        SaveBehavior(MatchInPlay& inPlay, persistence::SaveArchive& saveArchive) noexcept;

        /** @brief Whether there is any saved match at all. */
        [[nodiscard]] bool hasSavedMatch() const;

        /** @brief Every save there is, newest first. */
        [[nodiscard]] std::vector<persistence::SaveSummary> savedMatches() const;

        /** @brief What the match in play is already called, when it came from a save. */
        [[nodiscard]] std::string nameInPlay() const;

        /** @brief Writes the match in play away under @p name, updating the save it came
         * from when it came from one, and making a new one when it did not. */
        [[nodiscard]] SaveOutcome saveMatch(const std::string& name);

        /** @brief Replaces whatever is in play with the save called @p name.
         * @return whether that save was there and could be read. */
        [[nodiscard]] bool loadMatch(const std::string& name);

        /** @brief Throws the save called @p name away. @return whether one was there. */
        bool deleteSave(const std::string& name);

    private:
        MatchInPlay& inPlay_;
        persistence::SaveArchive& saveArchive_;
    };
}  // namespace cpp_warships::model::behaviors
