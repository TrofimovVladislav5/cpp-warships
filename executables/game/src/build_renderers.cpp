#include <build_renderers.h>

#include <memory>

#include <application/head/views/BattleView.h>
#include <application/head/views/MenuView.h>
#include <application/head/views/PlacementView.h>
#include <application/head/views/plain/PlainBattleView.h>
#include <application/head/views/plain/PlainMenuView.h>
#include <application/head/views/plain/PlainPlacementView.h>

namespace cpp_warships::application {
    namespace {
        /** @brief Drawn for an interactive terminal: colour, borders and a mouse. */
        [[nodiscard]] head::RendererSet interactiveRenderers(head::PresentationContext& context) {
            head::RendererSet renderers;

            renderers.drawScreenWith(
                    head::ScreenKind::Menu,
                    std::make_unique<head::MenuView>(context)
            );
            renderers.drawScreenWith(
                    head::ScreenKind::Placement,
                    std::make_unique<head::PlacementView>(context, context.geometry())
            );
            renderers.drawScreenWith(
                    head::ScreenKind::Battle,
                    std::make_unique<head::BattleView>(context, context.geometry())
            );

            return renderers;
        }

        /** @brief Printed as plain text, the way the console game used to read. */
        [[nodiscard]] head::RendererSet plainRenderers(head::PresentationContext& context) {
            head::RendererSet renderers;

            renderers.drawScreenWith(
                    head::ScreenKind::Menu,
                    std::make_unique<head::PlainMenuView>(context)
            );
            renderers.drawScreenWith(
                    head::ScreenKind::Placement,
                    std::make_unique<head::PlainPlacementView>(context)
            );
            renderers.drawScreenWith(
                    head::ScreenKind::Battle,
                    std::make_unique<head::PlainBattleView>(context)
            );

            return renderers;
        }
    } // namespace

    head::RendererSet buildRenderers(const ShellKind kind, head::PresentationContext& context) {
        return kind == ShellKind::PlainTerminal ? plainRenderers(context)
                                                : interactiveRenderers(context);
    }
} // namespace cpp_warships::application
