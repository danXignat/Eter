#pragma once
#include <QDebug>
#include <QtWidgets/QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include "ui_GameGUI.h"
#include "../EterV2/Board.h"
#include "../EterV2/Player.h"

class Card : public QGraphicsItem {
public:
    explicit Card(const QString& imagePath, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QPixmap cardImage;
    QPointF lastMousePosition; // Tracks the last mouse position for movement
};

class GameGUI : public QMainWindow {
    Q_OBJECT

public:
    GameGUI(base::Board& board, base::Player& redPlayer, base::Player& bluePlayer, QWidget* parent = nullptr);
    ~GameGUI();

    void drawSquareAt(QGraphicsScene* scene, int row, int col);
    void drawAvailablePositions();
    void drawImageAt(QGraphicsScene* scene, int row, int col, const QString& imagePath);
    void drawPlayerCards(const base::Player& player, int xStart, int yStart);

protected:

private:
    base::Board& gameBoard;
    QGraphicsScene* scene;
    QGraphicsView* view;
   
};
