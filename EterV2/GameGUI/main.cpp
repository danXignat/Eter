#include <QtWidgets/QApplication>

#include "MainWindow.h"
#include "../EterV2/Board.h"
#include "../EterV2/Player.h"
#include "../EterV2/GameModeType.h"
#include "../EterV2/Config.h"

int main(int argc, char *argv[])
{
    QApplication app{ argc, argv };
    QPixmap cursorPixmap("../pictures/estetics/custom_cursor.png");

    cursorPixmap = cursorPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QCursor customCursor(cursorPixmap, 0, 0);
    QApplication::setOverrideCursor(customCursor);
    base::Config::getInstance()
        .setStartPoint({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 })
        .setCardSpacingX(CARD_SIZE + CARD_OFFSET)
        .setCardSpacingY(CARD_SIZE + CARD_OFFSET);

    MainWindow window;
    window.show();

    return app.exec();
}
