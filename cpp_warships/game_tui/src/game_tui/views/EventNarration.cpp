#include <game_tui/views/EventNarration.h>

#include <functional>
#include <unordered_map>

#include <game_tui/views/CoordinateLabel.h>

namespace cpp_warships::game_tui {
    namespace {
        using Narrator = std::function<EventLine(const game_flow::MatchEvent&, const Theme&)>;

        const std::unordered_map<game_flow::SkillKind, std::string> NAME_BY_SKILL{
                {game_flow::SkillKind::Scanner, "scanner"},
                {game_flow::SkillKind::DoubleDamage, "double damage"},
                {game_flow::SkillKind::RandomStrike, "random strike"}
        };

        std::string actorName(game_flow::Participant actor) {
            return actor == game_flow::Participant::Player ? "you" : "the enemy";
        }

        std::string whereOf(const game_flow::MatchEvent& event) {
            return event.coordinate.has_value() ? coordinateLabel(*event.coordinate) : "nowhere";
        }

        const std::unordered_map<game_flow::MatchEventKind, Narrator> NARRATOR_BY_KIND{
                {game_flow::MatchEventKind::ShotMissed,
                 [](const game_flow::MatchEvent& event, const Theme& theme) {
                     return EventLine{
                             actorName(event.actor) + " missed at " + whereOf(event),
                             theme.miss.fill
                     };
                 }},
                {game_flow::MatchEventKind::ShipDamaged,
                 [](const game_flow::MatchEvent& event, const Theme& theme) {
                     return EventLine{
                             actorName(event.actor) + " struck a hull at " + whereOf(event),
                             theme.damaged.fill
                     };
                 }},
                {game_flow::MatchEventKind::ShipSunk,
                 [](const game_flow::MatchEvent& event, const Theme& theme) {
                     return EventLine{
                             actorName(event.actor) + " sank a ship at " + whereOf(event),
                             theme.sunk.fill
                     };
                 }},
                {game_flow::MatchEventKind::ShotRejected,
                 [](const game_flow::MatchEvent& event, const Theme& theme) {
                     return EventLine{
                             whereOf(event) + " has been fired on already",
                             theme.textMuted
                     };
                 }},
                {game_flow::MatchEventKind::SkillGranted,
                 [](const game_flow::MatchEvent& event, const Theme& theme) {
                     const std::string granted =
                             event.skill.has_value() ? skillName(*event.skill) : "a skill";
                     return EventLine{"earned " + granted, theme.accent};
                 }},
                {game_flow::MatchEventKind::DoubleDamageArmed,
                 [](const game_flow::MatchEvent&, const Theme& theme) {
                     return EventLine{"the next shot will bite twice", theme.accent};
                 }},
                {game_flow::MatchEventKind::AreaScanned,
                 [](const game_flow::MatchEvent& event, const Theme& theme) {
                     const std::string finding =
                             event.scanFoundShip ? " found a hull" : " found open water";
                     return EventLine{
                             "the scan around " + whereOf(event) + finding,
                             event.scanFoundShip ? theme.damaged.fill : theme.textMuted
                     };
                 }},
                {game_flow::MatchEventKind::RoundWon,
                 [](const game_flow::MatchEvent&, const Theme& theme) {
                     return EventLine{"enemy fleet destroyed, a new one sails in", theme.success};
                 }},
                {game_flow::MatchEventKind::MatchLost,
                 [](const game_flow::MatchEvent&, const Theme& theme) {
                     return EventLine{"your fleet is gone", theme.danger};
                 }},
                {game_flow::MatchEventKind::TurnPassed,
                 [](const game_flow::MatchEvent& event, const Theme& theme) {
                     const bool isPlayerNext = event.actor == game_flow::Participant::Player;
                     return EventLine{
                             isPlayerNext ? "your turn to fire" : "the enemy takes aim",
                             theme.textMuted
                     };
                 }}
        };
    } // namespace

    std::string skillName(game_flow::SkillKind skill) {
        return NAME_BY_SKILL.at(skill);
    }

    EventLine narrate(const game_flow::MatchEvent& event, const Theme& theme) {
        return NARRATOR_BY_KIND.at(event.kind)(event, theme);
    }
} // namespace cpp_warships::game_tui
