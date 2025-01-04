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
        .setStartPoint({ 600, 400 })
        .setCardSpacingX(100)
        .setCardSpacingY(100);

    base::Player player1("Player 1", color::ColorType::RED, base::GameSizeType::BIG);
    base::Player player2("Player 2", color::ColorType::BLUE, base::GameSizeType::BIG);

    base::Board board(20, player1, player2);

    GameGUI gui(board, player1, player2);
    gui.show();
    return a.exec();
}
