#pragma once
#include <ranges>
#include "qt_includes.h"
#include "..\EterV2\GameModeFactory.h"
#include "items.h"
#include "utils.h"

class GameView : public QGraphicsView {
	Q_OBJECT

signals:
    void cardAppend(Card* card);
    void explosionDiscard();
    void explosionApply();
    void explosionRotateLeft();
    void explosionRotateRight();

public:
	GameView(const QString& name_red, const QString& name_blue, QWidget* parent = nullptr);

    void drawPlayerCards(const base::Player& player, QPointF start_point);
    void drawAvailablePositions(const base::Board& board);
    void drawExplosion(const base::ExplosionService& effect, uint16_t, QPointF start_point);
    void drawHole(const QPointF& pos);
    void cardAppendBoard(Card* card);
    void switchToPlayer(const base::Player& player);
    void showWin(color::ColorType color);
    void setExplosionActive(const QPointF& p1, const QPointF& p2);
    void eraseExplosion();
    QHash<uint16_t, Card*>& getAllCards();
    void keyPressEvent(QKeyEvent* event);
private:
    void _drawSquareAt(QPointF pos);
    void _initLabels(const QString& name_red, const QString& name_blue);
    Card* _createCardAt(color::ColorType color, base::CombatCardType type, QPointF pos, uint16_t id);
    
private:
    QVBoxLayout* layout;
    QLabel* infoLabel;
    QLabel* red_name_label;
    QLabel* blue_name_label;
    QLabel* won_label;

    QGraphicsScene* scene;
    Explosion*      explosion;
    TargetZone*     explosion_target_zone;
    Vortex*         vortex;

    QList<Card*> red_deck;
    QList<Card*> blue_deck;
    QHash<QPointF, BoardCell*> m_available_cells;

    QHash<QPointF, QList<Card*>> board_cards;
    QHash<uint16_t, Card*> all_cards;
};

class GameController : public QObject{
	Q_OBJECT

public:
    GameController(QWidget* parent,
        const std::string& mode,
        const QString& name_red, const QString& name_blue);
private:
    void _updateBoardView();
    void _updatePlayerCards(const base::Player& player, QHash<uint16_t, Card*>& card_views, bool hide);
    void _updateBoardCards(const base::Board& board, QHash<uint16_t, Card*>& card_views);
    void _initVisuals();
    void _initConections();

private:
    base::GameModePtr model;
    GameView* view;

public slots:
    void onCardAppend(Card* card);
    void onExplosionApply();
    void onExplosionDiscard();
    void onExplosionRotateLeft();
    void onExplosionRotateRight();
};