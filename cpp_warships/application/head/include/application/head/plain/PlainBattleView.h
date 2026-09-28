#pragma once

#include <application/head/common/PresentationContext.h>
#include <application/head/common/Queries.h>
#include <application/head/common/Theme.h>
#include <application/head/common/render/Renderer.h>
#include <application/head/common/state/BattleState.h>
#include <application/model/BattleJournal.h>

namespace cpp_warships::head {
    /** @brief The battle printed the way the console game used to print it:
     * both boards one under the other, then the skills in the bank and the
     * story so far.
     */
    class PlainBattleView final : public Renderer {
       public:
        explicit PlainBattleView(const PresentationContext& context) noexcept;

        [[nodiscard]] Frame render(int availableWidth, int availableHeight) override;

       private:
        const PresentationContext& context_;
    };
}  // namespace cpp_warships::head
