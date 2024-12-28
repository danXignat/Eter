#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GameGUI.h"
#include "../EterV2/Board.h"
#include "../EterV2/Player.h"


class GameGUI : public QMainWindow
{
    Q_OBJECT

private:
    Ui::GameGUIClass ui;
    base::Board& gameBoard;
    QGraphicsScene* scene;
protected:
    void mousePressEvent(QMouseEvent* event) override;
public:
    GameGUI(base::Board& board, base::Player& redPlayer, base::Player& bluePlayer, QWidget* parent = nullptr);
    ~GameGUI();

    void drawSquareAt(QGraphicsScene* scene, int row, int col);
    void drawAvailablePositions();
    void drawImageAt(QGraphicsScene* scene, int row, int col, const QString& imagePath);
    void drawPlayerCards(const base::Player& player, int xStart, int yStart);

   
};

