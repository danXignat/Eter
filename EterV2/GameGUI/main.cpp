#include "GameGUI.h"
#include "../EterV2/Board.h"
#include "../EterV2/Player.h"
#include "../EterV2/GameModeType.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    base::Player player1("Player 1", color::ColorType::RED, base::GameSizeType::SMALL);
    base::Player player2("Player 2", color::ColorType::BLUE, base::GameSizeType::SMALL);

    base::Board board(20, player1, player2);

    GameGUI gui(board);
    gui.show();
    return a.exec();
}
