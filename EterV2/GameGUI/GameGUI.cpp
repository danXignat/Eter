#include "GameGUI.h"
#include <QGraphicsScene>
#include "../EterV2/Board.h"
#include "../EterV2/Player.h"   
#include <QMouseEvent>

GameGUI::GameGUI(base::Board& board, QWidget* parent)
    : QMainWindow(parent), gameBoard(board), scene(new QGraphicsScene(this)){

    ui.setupUi(this);
    ui.graphicsViewGameBoard->setScene(scene);

    drawAvailablePositions();
}

GameGUI::~GameGUI()
{}

void GameGUI::drawSquareAt(QGraphicsScene* scene, int row, int col) {

    int x = col * 50;
    int y = row * 25;

    QRectF cellRect(x, y, 50, 50);
    QBrush cellBrush(Qt::transparent);
    scene->addRect(cellRect, QPen(Qt::white), cellBrush);

}
void GameGUI::drawAvailablePositions() {

    //scene->clear();

    const auto& availableSpaces = gameBoard.availableSpaces();

    for (const auto& coord : availableSpaces) {
        drawSquareAt(scene, coord.first, coord.second);
    }
}

void GameGUI::mousePressEvent(QMouseEvent* event) {

    QPointF scenePos = ui.graphicsViewGameBoard->mapToScene(event->pos());

    std::pair<int16_t, int16_t> coords;
    coords.first = static_cast<int>(scenePos.y()) / 50;
    coords.second = static_cast<int>(scenePos.x()) / 50;

    coords.first *= 2;
    base::CombatCard card(base::CombatCardType::ONE, color::ColorType::RED);
    const auto& availableSpaces = gameBoard.availableSpaces();
    for (const auto& coord : availableSpaces){
        if (coords.first == coord.first && coords.second == coord.second) {
            gameBoard.appendAnyCard(coords, std::move(card));
            drawAvailablePositions();
            break;
        }
    }
}