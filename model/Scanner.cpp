#include "Scanner.h"
#include "GameFieldView.h"
Scanner::Scanner(const std::pair<int,int>& leftUpper, MatchSettings* settings)
    : settings(settings)
    , leftUpper(leftUpper)
{}

void Scanner::apply() {
    GameFieldView view(settings->getOpponentField());
    view.displayScan(leftUpper);
}