#pragma once

#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/state/BattleState.h>
#include <application/model/BattleJournal.h>
#include <application/head/PresentationContext.h>
#include <application/head/views/Renderer.h>

namespace cpp_warships::head {
    /** @brief The battle printed the way the console game used to print it: both boards
     *  one under the other, then the skills in the bank and the story so far. */
    class PlainBattleView final : public Renderer {
    public:
        explicit PlainBattleView(const PresentationContext& context) noexcept;

        [[nodiscard]] Frame render(int availableWidth, int availableHeight) override;


    private:
        const PresentationContext& context_;
    };
} // namespace cpp_warships::head
