#include <application/model/BattleJournal.h>

namespace cpp_warships::model {
    namespace {
        constexpr std::size_t REMEMBERED_EVENT_COUNT = 200;
    } // namespace

    void BattleJournal::absorb(const flow::MatchEventLog& events) {
        entries_.insert(entries_.end(), events.begin(), events.end());

        while (entries_.size() > REMEMBERED_EVENT_COUNT) {
            entries_.pop_front();
        }
    }

    void BattleJournal::clear() noexcept {
        entries_.clear();
    }

    const std::deque<flow::MatchEvent>& BattleJournal::entries() const noexcept {
        return entries_;
    }

    bool BattleJournal::isEmpty() const noexcept {
        return entries_.empty();
    }
} // namespace cpp_warships::model
