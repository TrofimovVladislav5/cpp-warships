#include "Scanner.h"
#include "GameFieldView.h"
Scanner::Scanner(std::pair<int,int>& leftUpper, MatchSettings* settings)
    : leftUpper(leftUpper)
    , settings(settings)
{}

void Scanner::apply() {
    GameFieldView view(settings->getOpponentField());
    view.displayScan(leftUpper);
}