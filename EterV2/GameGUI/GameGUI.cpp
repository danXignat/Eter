#include "GameGUI.h"
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "../EterV2/Board.h"
#include "../EterV2/Player.h"  
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <sstream>
#include <iostream>
#include <qdebug.h>

GameGUI::GameGUI(base::Board& board, base::Player& redPlayer, base::Player& bluePlayer, QWidget* parent)
    : QMainWindow(parent), gameBoard(board), scene(new QGraphicsScene(this)) {

    ui.setupUi(this);
    ui.graphicsViewGameBoard->setScene(scene);

    drawAvailablePositions();

    // Call renderPlayerCards for both players
    drawPlayerCards(redPlayer, 45, 100);
    drawPlayerCards(bluePlayer, 45, 100);
}

GameGUI::~GameGUI()
{
}

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
void GameGUI::drawImageAt(QGraphicsScene* scene, int row, int col, const QString& imagePath) {
    int x = col * 50;
    int y = row * 50;
    QPixmap pixmap(imagePath);
    QPixmap scaledPixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem* item = scene->addPixmap(scaledPixmap);
    item->setPos(x, y);
}

void GameGUI::drawPlayerCards(const base::Player& player, int xStart, int yStart) {
    int x_red = xStart;
    int x_blue = xStart;
    int y = yStart;

    const auto& cards = player.getCards();

    if (player.getColor() == color::ColorType::RED) {
        y = 50;
    }
    else if (player.getColor() == color::ColorType::BLUE) {
        y = scene->height() + 100;
    }

    for (const auto& [type, card] : cards) {
        QString imagePath = QString("C:/Eter/EterV2/eterCards/%1_%2.png")
            .arg(player.getColor() == color::ColorType::RED ? "red" : "blue")
            .arg(combatCardToChar(type));

        if (player.getColor() == color::ColorType::RED) {
            drawImageAt(scene, y / 50, x_red / 50, imagePath);
            x_red += 50; 
        }
        else {
            drawImageAt(scene, y / 50, x_blue / 50, imagePath);
            x_blue += 50;
        }
    }
}


void GameGUI::mousePressEvent(QMouseEvent* event) {

    QPointF scenePos = ui.graphicsViewGameBoard->mapToScene(event->pos());

    std::pair<int16_t, int16_t> coords;
    coords.first = static_cast<int>(scenePos.y()) / 50;
    coords.second = static_cast<int>(scenePos.x()) / 50;

    coords.first *= 2; //si pe mine ma deranjeaza enorm ca merge, nu stiu de ce o face

    base::CombatCard card(base::CombatCardType::ONE, color::ColorType::RED);
    const auto& availableSpaces = gameBoard.availableSpaces();
    for (const auto& coord : availableSpaces) {
        if (coords.first == coord.first && coords.second == coord.second) {
            drawImageAt(scene, coords.first / 2, coords.second, "C:/Eter/EterV2/eterCards/1_blue.png");
            gameBoard.appendAnyCard(coords, std::move(card));
            drawAvailablePositions();
            break;
        }
    }
}
