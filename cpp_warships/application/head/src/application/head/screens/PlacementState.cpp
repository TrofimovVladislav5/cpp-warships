#include <application/head/screens/PlacementState.h>

namespace cpp_warships::head {
    namespace {
        int longestLengthLeft(const flow::PlacementPlan& plan) {
            int longest = 0;
            for (const auto& [length, remaining] : plan.remaining()) {
                if (remaining > 0) {
                    longest = length;
                }
            }

            return longest;
        }

        int shortestLengthLeft(const flow::PlacementPlan& plan) {
            for (const auto& [length, remaining] : plan.remaining()) {
                if (remaining > 0) {
                    return length;
                }
            }

            return 0;
        }
    } // namespace

    int shipLengthInHand(const flow::PlacementPlan& plan, const PlacementState& state) {
        if (plan.remainingOf(state.preferredShipLength) > 0) {
            return state.preferredShipLength;
        }

        return longestLengthLeft(plan);
    }

    int nextShipLength(const flow::PlacementPlan& plan, int currentLength) {
        for (const auto& [length, remaining] : plan.remaining()) {
            if (remaining > 0 && length > currentLength) {
                return length;
            }
        }

        return shortestLengthLeft(plan);
    }
} // namespace cpp_warships::head
