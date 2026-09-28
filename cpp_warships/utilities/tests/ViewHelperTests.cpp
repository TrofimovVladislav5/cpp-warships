#include <gtest/gtest.h>
#include <utilities/StateMessages.h>
#include <utilities/ViewHelper.h>

#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {
    /** @brief Captures whatever is written to a stream while it is in scope. */
    class StreamCapture {
    public:
        explicit StreamCapture(std::ostream& stream)
            : stream_(stream)
            , original_(stream.rdbuf(captured_.rdbuf())) {}

        StreamCapture(const StreamCapture&) = delete;
        StreamCapture& operator=(const StreamCapture&) = delete;

        ~StreamCapture() {
            stream_.rdbuf(original_);
        }

        [[nodiscard]] std::string text() const {
            return captured_.str();
        }

    private:
        std::ostream& stream_;
        std::ostringstream captured_;
        std::streambuf* original_;
    };

    /** @brief Answers a prompt with whatever it was built with. */
    std::function<std::string()> answering(const std::string& answer) {
        return [answer]() { return answer; };
    }
}  // namespace

TEST(ViewHelperTests, WritesWhatItIsGivenToTheConsole) {
    const StreamCapture capture{std::cout};

    ViewHelper::consoleOut("a message");

    EXPECT_NE(capture.text().find("a message"), std::string::npos);
}

TEST(ViewHelperTests, IndentsByTheLevelAskedFor) {
    const StreamCapture capture{std::cout};

    ViewHelper::consoleOut("indented", 2);

    EXPECT_NE(capture.text().find("\t\tindented"), std::string::npos);
}

TEST(ViewHelperTests, WritesAnErrorToTheErrorStream) {
    const StreamCapture capture{std::cerr};

    ViewHelper::errorOut("something broke");

    EXPECT_NE(capture.text().find("something broke"), std::string::npos);
}

TEST(ViewHelperTests, WritesWhatAnExceptionSaidAlongsideTheError) {
    const StreamCapture capture{std::cerr};

    ViewHelper::errorOut("something broke", std::runtime_error{"the reason"});

    EXPECT_NE(capture.text().find("something broke"), std::string::npos);
    EXPECT_NE(capture.text().find("the reason"), std::string::npos);
}

TEST(ViewHelperTests, ConfirmsWhenTheAnswerMatches) {
    const StreamCapture capture{std::cout};

    EXPECT_TRUE(ViewHelper::confirmAction(answering("yes"), "yes"));
}

TEST(ViewHelperTests, ConfirmsWhateverTheLetterCase) {
    const StreamCapture capture{std::cout};

    EXPECT_TRUE(ViewHelper::confirmAction(answering("YES"), "yes"));
    EXPECT_TRUE(ViewHelper::confirmAction(answering("yes"), "YES"));
}

TEST(ViewHelperTests, DoesNotConfirmWhenTheAnswerDiffers) {
    const StreamCapture capture{std::cout};

    EXPECT_FALSE(ViewHelper::confirmAction(answering("no"), "yes"));
    EXPECT_FALSE(ViewHelper::confirmAction(answering(""), "yes"));
}

TEST(ViewHelperTests, AsksBeforeWaitingForAnAnswer) {
    const StreamCapture capture{std::cout};

    ViewHelper::confirmAction(answering("yes"), "yes");

    EXPECT_NE(capture.text().find("confirm"), std::string::npos);
}

TEST(StateMessagesTests, NamesTheStateBeingEntered) {
    const StreamCapture capture{std::cout};

    StateMessages::displayGreetingMessage("battle");

    EXPECT_NE(capture.text().find("battle"), std::string::npos);
    EXPECT_NE(capture.text().find("help"), std::string::npos);
}

TEST(StateMessagesTests, NamesTheStateBeingLeft) {
    const StreamCapture capture{std::cout};

    StateMessages::displayCloseMessage("battle");

    EXPECT_NE(capture.text().find("battle"), std::string::npos);
}

TEST(StateMessagesTests, SaysItIsWaiting) {
    const StreamCapture capture{std::cout};

    StateMessages::awaitCommandMessage();

    EXPECT_NE(capture.text().find("Waiting"), std::string::npos);
}
