#pragma once

#include "ISerializable.h"
#include "MatchSettings.h"
#include "ShipManager.h"
#include "player_skills/SkillManager.h"

namespace cpp_warships::application {
    inline char GameStateDTOName[] = "GameStateDTO";

    class GameStateDTO
        : public game_saves::ISerializable<GameStateDTOName>
    {
    public:
        explicit GameStateDTO();
        GameStateDTO(const GameStateDTO& other) noexcept;
        ~GameStateDTO() override = default;

        /**
         *
         * @param settings Settings for the new game
         * @return A vector of pointers to the initialized objects so the memory
         * can be managed outside of this class.
         */
        std::vector<void*> initiateNewGame(MatchSettings *settings);

        ShipManager* playerManager;
        SkillManager* playerSkillManager;
        GameField* playerField;
        ShipManager* enemyManager;
        GameField* enemyField;
        std::map<int, int> shipsSizes;
        MatchSettings* settings;
        bool isFinished;
        int fieldSize;
        int roundNumber;
    };
} // namespace cpp_warships::application