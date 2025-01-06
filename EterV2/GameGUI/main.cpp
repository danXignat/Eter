#include <QtWidgets/QApplication>

#include "GameGUI.h"
#include "../EterV2/Board.h"
#include "../EterV2/Player.h"
#include "../EterV2/GameModeType.h"
#include "../EterV2/Config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    base::Config::getInstance()
        .setStartPoint({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 })
        .setCardSpacingX(CARD_SIZE)
        .setCardSpacingY(CARD_SIZE);

    GameGUI gui;
    gui.show();

    return a.exec();
}
