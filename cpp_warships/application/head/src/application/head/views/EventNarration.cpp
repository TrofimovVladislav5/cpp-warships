#include <application/head/views/EventNarration.h>

#include <functional>
#include <unordered_map>

#include <application/head/views/CoordinateLabel.h>

namespace cpp_warships::head {
    namespace {
        using Narrator = std::function<EventLine(const flow::MatchEvent&, const Theme&)>;

        const std::unordered_map<flow::SkillKind, std::string> NAME_BY_SKILL{
                {flow::SkillKind::Scanner, "scanner"},
                {flow::SkillKind::DoubleDamage, "double damage"},
                {flow::SkillKind::RandomStrike, "random strike"}
        };

        std::string actorName(flow::Participant actor) {
            return actor == flow::Participant::Player ? "you" : "the enemy";
        }

        std::string whereOf(const flow::MatchEvent& event) {
            return event.coordinate.has_value() ? coordinateLabel(*event.coordinate) : "nowhere";
        }

        const std::unordered_map<flow::MatchEventKind, Narrator> NARRATOR_BY_KIND{
                {flow::MatchEventKind::ShotMissed,
                 [](const flow::MatchEvent& event, const Theme& theme) {
                     return EventLine{
                             actorName(event.actor) + " missed at " + whereOf(event),
                             theme.miss.fill
                     };
                 }},
                {flow::MatchEventKind::ShipDamaged,
                 [](const flow::MatchEvent& event, const Theme& theme) {
                     return EventLine{
                             actorName(event.actor) + " struck a hull at " + whereOf(event),
                             theme.damaged.fill
                     };
                 }},
                {flow::MatchEventKind::ShipSunk,
                 [](const flow::MatchEvent& event, const Theme& theme) {
                     return EventLine{
                             actorName(event.actor) + " sank a ship at " + whereOf(event),
                             theme.sunk.fill
                     };
                 }},
                {flow::MatchEventKind::ShotRejected,
                 [](const flow::MatchEvent& event, const Theme& theme) {
                     return EventLine{
                             whereOf(event) + " has been fired on already",
                             theme.textMuted
                     };
                 }},
                {flow::MatchEventKind::SkillGranted,
                 [](const flow::MatchEvent& event, const Theme& theme) {
                     const std::string granted =
                             event.skill.has_value() ? skillName(*event.skill) : "a skill";
                     return EventLine{"earned " + granted, theme.accent};
                 }},
                {flow::MatchEventKind::DoubleDamageArmed,
                 [](const flow::MatchEvent&, const Theme& theme) {
                     return EventLine{"the next shot will bite twice", theme.accent};
                 }},
                {flow::MatchEventKind::AreaScanned,
                 [](const flow::MatchEvent& event, const Theme& theme) {
                     const std::string finding =
                             event.scanFoundShip ? " found a hull" : " found open water";
                     return EventLine{
                             "the scan around " + whereOf(event) + finding,
                             event.scanFoundShip ? theme.damaged.fill : theme.textMuted
                     };
                 }},
                {flow::MatchEventKind::RoundWon,
                 [](const flow::MatchEvent&, const Theme& theme) {
                     return EventLine{"enemy fleet destroyed, a new one sails in", theme.success};
                 }},
                {flow::MatchEventKind::MatchLost,
                 [](const flow::MatchEvent&, const Theme& theme) {
                     return EventLine{"your fleet is gone", theme.danger};
                 }},
                {flow::MatchEventKind::TurnPassed,
                 [](const flow::MatchEvent& event, const Theme& theme) {
                     const bool isPlayerNext = event.actor == flow::Participant::Player;
                     return EventLine{
                             isPlayerNext ? "your turn to fire" : "the enemy takes aim",
                             theme.textMuted
                     };
                 }}
        };
    } // namespace

    std::string skillName(flow::SkillKind skill) {
        return NAME_BY_SKILL.at(skill);
    }

    EventLine narrate(const flow::MatchEvent& event, const Theme& theme) {
        return NARRATOR_BY_KIND.at(event.kind)(event, theme);
    }
} // namespace cpp_warships::head
