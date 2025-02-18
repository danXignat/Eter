#pragma once

#include <ranges>
#include "..\EterV2\GameModeFactory.h"
#include "qt_includes.h"
#include "items.h"
#include "utils.h"

class GameView : public QGraphicsView {
    Q_OBJECT

signals:
    void cardAppend(Card* card);
    void selectIllusion(Card* card);
    void cardStartMoving(Card* card);
    void cardStoppedMoving(Card* card);
    void clickedOnRemove(Card* card);

    void explosionDiscard();
    void explosionApply();
    void explosionRotateLeft();
    void explosionRotateRight();

    void applyMageCard(MageCard* card);
    void applyPowerCard(PowerCard* card);

    void nextRound();

public:
    GameView(const QString& name_red, const QString& name_blue, QWidget* parent = nullptr);

    void drawPlayerCards(const base::Player& player, QPointF start_point);
    void drawAvailablePositions(const base::Board& board);
    void drawExplosion          (const base::ExplosionService& effect, uint16_t, QPointF start_point);
    void drawHole       (const QPointF& pos);
    void drawMages      (base::MageTypeAbility mage_red, const QString& desc_red, base::MageTypeAbility mage_blue, const QString& desc_blue);
    void drawArena      (base::GameSizeType game_size);
    void drawMarker(const QPointF& point, color::ColorType color);
    void drawPowers(const std::pair<std::pair<uint16_t, base::PowerCardType>, std::pair<uint16_t, base::PowerCardType>>& data);

    void cardAppendBoard    (Card* card);
    void setExplosionActive ();
    void eraseExplosion     ();

    void switchToPlayer(const base::Player& player);
    void showWin(color::ColorType color);

    QHash<uint16_t, Card*>& getAllCards();
    QHash<QPointF, QList<Card*>>& getAllCardsByPos();
    TargetZone* getTargetZone();

    void mousePressEvent    (QMouseEvent* event) override;
    void mouseMoveEvent     (QMouseEvent* event) override;
    void mouseReleaseEvent  (QMouseEvent* event) override;
    void keyPressEvent      (QKeyEvent* event);

    void highlightCards         (const std::unordered_set<uint16_t>& valid_choices, CardState state1, CardState state2);
    void resetCardHighlight     ();
    void showCards              (std::function<bool(Card*)> condition);
    void hideCards              (std::function<bool(Card*)> condition);
    void setDeckCardsMovability (color::ColorType color, bool move);
    void makeCardsMovable       (const std::unordered_set<uint16_t>& ids, bool move);
    void setStackMoveEvent      (const std::unordered_set<uint16_t>& ids, bool in_event);
    void moveCards              (const std::unordered_set<uint16_t>& ids, const base::Coord& to);
    void connectCards           ();

    void drawAvailableSpaces    (const std::unordered_set<base::Coord, base::utils::CoordFunctor>& coords);
    void showAvailableSpaces    (std::unordered_set<base::Coord, base::utils::CoordFunctor> coords);
    void showAllAvailableSpaces ();
    void hideAllAvailableSpaces ();

public:
    bool red_illusion_in_use  {false};
    bool blue_illusion_in_use {false};

    QGraphicsScene* scene;
    TargetZone* target_zone;

private:
    void _drawSquareAt(QPointF pos);
    void _initLabels(const QString& name_red, const QString& name_blue);
    Card* _createCardAt(color::ColorType color, base::CombatCardType type, QPointF pos, uint16_t id);

private:
    VictoryScreen* victory_screen;

    QVBoxLayout*    layout;
    QLabel*         infoLabel;
    QLabel*         red_name_label;
    QLabel*         blue_name_label;
    QLabel*         won_label;

    QList<Card*> red_deck;
    QList<Card*> blue_deck;

    QHash<QPointF, QList<Card*>> board_cards;
    QHash<uint16_t, Card*>       all_cards;
    QHash<QPointF, BoardCell*>   m_available_cells;

    Explosion*  explosion;
    Vortex*     vortex;

    MageCard* mage_card_red;
    MageCard* mage_card_blue;

    PowerCard* power_card1;
    PowerCard* power_card2;

    Arena* arena;
    QList<Marker*> markers;
};