#pragma once
#include <QDebug>
#include <QtWidgets/QMainWindow>

#include "qt_includes.h"
#include "settings.h"

#include "..\EterV2\GameModeFactory.h"
#include "..\EterV2\InputHandler.h"

class Card : public QObject, public QGraphicsItem {
    Q_OBJECT

public:
    explicit Card(color::ColorType, base::CombatCardType, const QString& imagePath, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QPixmap cardImage;
    QPointF lastMousePosition;
    color::ColorType color;
    base::CombatCardType type;

signals:
    void cardAppend(color::ColorType, base::CombatCardType, QPoint coord);
};

class Board : public QGraphicsItem {

};

class GameGUI : public QMainWindow {
    Q_OBJECT

public:
    GameGUI(QWidget* parent = nullptr);
    ~GameGUI();

    void createCardAt(color::ColorType color, base::CombatCardType type, QPointF pos);
    void drawPlayerCards(const base::Player& player, QPointF start_point);
    void drawSquareAt(QPoint pos);
    void drawAvailablePositions();

protected:

public slots:
    void onCardAppend(color::ColorType, base::CombatCardType, QPoint coord);
    void onTrainingModeSelected();
    void onMageDuelModeSelected();
    void onElementalBattleModeSelected();
private:
    base::GameModePtr gamemode;
    /*const base::Player& player_one;
    const base::Player& player_two;
    const base::Board& game_board;*/

    QGraphicsScene* scene;
    QGraphicsView* view;
private:
    void initializeGameMode(const std::string& modeId);
};
