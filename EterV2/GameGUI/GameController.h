#pragma once
#include <ranges>

#include "..\EterV2\GameModeFactory.h"


#include "GameView.h"
#include "qt_includes.h"
#include "settings.h"
#include "items.h"
#include "utils.h"

class MageController : public QObject {
    Q_OBJECT
signals:
    void switchPlayer();
    void updateCards();
    void cardAppend(Card* card);

public:
    MageController(QObject* parent, base::MageService& service, GameView* view);

    void onMageCardApply(MageCard* card);
    void selectMage(MageCard* card);

    void handleFireFront(color::ColorType color);
    void handleFireBack(color::ColorType color);

    void handleEarthFront(color::ColorType color);
    void handleEarthBack(color::ColorType color);

    void handleAirFront(color::ColorType color);
    void handleAirBack(color::ColorType color);

    void handleWaterFront(color::ColorType color);
    void handleWaterBack(color::ColorType color);

private:
    base::MageService& m_service;
    GameView* m_view;

};

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
};