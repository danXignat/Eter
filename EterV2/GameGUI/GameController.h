#pragma once
#include <ranges>

#include "..\EterV2\GameModeFactory.h"

#include "GameView.h"
#include "qt_includes.h"
#include "settings.h"
#include "items.h"
#include "utils.h"

#include "MageController.h"
#include "ElementalController.h"

class GameController : public QObject {
    Q_OBJECT

public:
    GameController(QWidget* parent,
        const std::string& mode,
        const QString& name_red, const QString& name_blue);
private:
    void _updateBoardView();
    void _updatePlayerCards(const base::Player& player, QHash<uint16_t, Card*>& card_views, bool hide);
    void _updateBoardCards(const base::Board& board,    QHash<uint16_t, Card*>& card_views);
    void _initVisuals();
    void _initConections();
    void _updateTargetZone();

private:
    base::GameModePtr model;
    GameView* view;

    MageController* m_mage_controller;
    ElementalController* m_elemental_controller;

public slots:
    void onCardAppend(Card* card);
    void onSelectIllusion(Card* card);
    void onCardStartMoving(Card* card);
    void onCardStoppedMoving(Card* card);

    void onExplosionApply();
    void onExplosionDiscard();
    void onExplosionRotateLeft();
    void onExplosionRotateRight();

    void onSwitchPlayer();
    void onUpdateCards();

    void onNextRound();
};