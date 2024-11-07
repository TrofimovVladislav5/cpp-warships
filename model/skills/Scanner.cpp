#include "Scanner.h"

#include <set>

#include "../../view/GameFieldView.h"
Scanner::Scanner(MatchSettings* settings)
    : settings(settings)
{
    // scannerView = new GameFieldView(settings->getOpponentField());
}

void Scanner::apply() {
    // scannerView->displayScan(settings->coordinateToScan());
}