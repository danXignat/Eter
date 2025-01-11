#include <QtWidgets/QApplication>

#include "MainWindow.h"
#include "../EterV2/Board.h"
#include "../EterV2/Player.h"
#include "../EterV2/GameModeType.h"
#include "../EterV2/Config.h"

int main(int argc, char *argv[])
{
    QApplication app{ argc, argv };
    
    base::Config::getInstance()
        .setStartPoint({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 })
        .setCardSpacingX(CARD_SIZE + CARD_OFFSET)
        .setCardSpacingY(CARD_SIZE + CARD_OFFSET);

    MainWindow window;
    window.show();

    return app.exec();
}
