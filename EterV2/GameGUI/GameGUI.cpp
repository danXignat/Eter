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
    : QMainWindow(parent),
    scene{ new QGraphicsScene(this) },
    view{ new QGraphicsView(scene, this) },
    gameBoard(board) {
    setWindowTitle("Eter");
    resize(1200, 800);

    view->setGeometry(0, 0, 1200, 800);
    view->setMinimumSize(1200, 800);

    drawAvailablePositions();
    drawPlayerCards(redPlayer, 200, 200);
    drawPlayerCards(bluePlayer, 200, 600);
}

GameGUI::~GameGUI() {
    delete scene;
}

void GameGUI::drawSquareAt(QGraphicsScene* scene, int row, int col) {
    int x = col * 50;
    int y = row * 25;

    QRectF cellRect(x, y, 50, 50);
    QBrush cellBrush(Qt::transparent);
    scene->addRect(cellRect, QPen(Qt::white), cellBrush);
}

void GameGUI::drawAvailablePositions() {
    const auto& availableSpaces = gameBoard.availableSpaces();

    for (const auto& coord : availableSpaces) {
        drawSquareAt(scene, coord.first, coord.second);
    }
}

void GameGUI::drawImageAt(QGraphicsScene* scene, int row, int col, const QString& imagePath) {
    int x = col * 50;
    int y = row * 50;

    QPixmap pixmap(imagePath);
    QPixmap scaledPixmap = pixmap.scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem* item = scene->addPixmap(scaledPixmap);
    item->setPos(x, y);
}

void GameGUI::drawPlayerCards(const base::Player& player, int xStart, int yStart) {
    int x = xStart;
    int y = yStart;

    const auto& cards = player.getCards();

    for (const auto& [type, card] : cards) {

        QString imagePath = QString("../eterCards/%1_%2.png")
            .arg(player.getColor() == color::ColorType::RED ? "red" : "blue")
            .arg(combatCardToChar(type));

        Card* card = new Card(imagePath);
        card->setPos(x, y);
        scene->addItem(card);

        x += 100;
    }
}

Card::Card(const QString& imagePath, QGraphicsItem* parent)
    : QGraphicsItem(parent), cardImage(imagePath) {
    if (cardImage.isNull()) {
        qWarning() << "Failed to load card image:" << imagePath;
    }
    
    // Enable the item to accept mouse events
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

QRectF Card::boundingRect() const {
    return QRectF(0, 0, cardImage.width(), cardImage.height());
}

void Card::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPixmap scaledImage = cardImage.scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter->drawPixmap(0, 0, scaledImage);
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    // Save the initial mouse position
    lastMousePosition = event->scenePos();
    QGraphicsItem::mousePressEvent(event); // Call base class implementation
}

void Card::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    // Calculate the mouse movement
    QPointF delta = event->scenePos() - lastMousePosition;
    // Move the card
    setPos(pos() + delta);
    // Update the last mouse position
    lastMousePosition = event->scenePos();
    QGraphicsItem::mouseMoveEvent(event);
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    qDebug() << "Card released at position:" << pos();
    QGraphicsItem::mouseReleaseEvent(event);
}