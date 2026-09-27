#pragma once

#include <string>

#include <application/core/errors/WarshipsException.h>

namespace cpp_warships::model {
    /** @brief Something the model was asked to do that it could not. Errors from the layers
     *  below are wrapped into these at the entry point, so nothing raw reaches the interface. */
    class ModelException : public core::WarshipsException {
    protected:
        explicit ModelException(const std::string& message);
    };

    /** @brief The match in play was reached for when there is none. */
    class NoMatchInPlayException final : public ModelException {
    public:
        NoMatchInPlayException();
    };

    /** @brief One intent could not be carried out. Carries what failed and why, which is
     *  what the interface shows rather than the error from whichever layer raised it. */
    class IntentFailedException final : public ModelException {
    public:
        IntentFailedException(const std::string& intentName, const std::string& cause);

        [[nodiscard]] const std::string& intentName() const noexcept;
        [[nodiscard]] const std::string& cause() const noexcept;

    private:
        std::string intentName_;
        std::string cause_;
    };

    /** @brief A scenario gave up part way through, leaving its later steps untaken. */
    class ScenarioAbortedException final : public ModelException {
    public:
        ScenarioAbortedException(
                const std::string& scenarioName,
                int completedSteps,
                const std::string& cause
        );

        [[nodiscard]] const std::string& scenarioName() const noexcept;

        /** @brief How many of its steps had gone through before it stopped. */
        [[nodiscard]] int completedSteps() const noexcept;
        [[nodiscard]] const std::string& cause() const noexcept;

    private:
        std::string scenarioName_;
        int completedSteps_;
        std::string cause_;
    };
} // namespace cpp_warships::model
