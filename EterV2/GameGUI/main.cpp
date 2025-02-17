#include <QtWidgets/QApplication>

#include "MainWindow.h"
#include "../EterV2/Config.h"

int main(int argc, char *argv[]) {
    QApplication app{ argc, argv };

    ///Cursor
    QPixmap cursorPixmap{ CURSOR_PATH };
    cursorPixmap = cursorPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QCursor customCursor{ cursorPixmap, 0, 0 };
    QApplication::setOverrideCursor(customCursor);

    ///Backend setup
    base::Config::getInstance()
        .setStartPoint({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 })
        .setCardSpacingX(CARD_SIZE + CARD_OFFSET)
        .setCardSpacingY(CARD_SIZE + CARD_OFFSET);

    ///Main window start-up
    MainWindow window;
    window.show();

    return app.exec();
}
