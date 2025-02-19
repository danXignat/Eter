#pragma once

#include "..\EterV2\GameModeFactory.h"

#include "GameView.h"
#include "qt_includes.h"
#include "settings.h"
#include "items.h"
#include "utils.h"

class ElementalController : public QObject {
    Q_OBJECT
signals:
    void switchPlayer();
    void updateCards();
    void addExplosion();
    void cardAppend(Card* card);
    void addCard();
    void getCurrPlayer(color::ColorType& color);

public:
    ElementalController(QObject* parent, base::ElementalService& service, GameView* view);

    void selectPower(PowerCard* card);
    void onPowerCardApply(PowerCard* card);

    void handleControllerExplosion(PowerCard* power_card_view);
    void handleDestruction(PowerCard* power_card_view);
    void handleFlame(PowerCard* power_card_view);
    void handleFire(PowerCard* power_card_view);
    void handleAsh(PowerCard* power_card_view);
    void handleSpark(PowerCard* power_card_view);
    void handleSquall(PowerCard* power_card_view);
    void handleGale(PowerCard* power_card_view);
    void handleHurricane(PowerCard* power_card_view);
    void handleGust(PowerCard* power_card_view);
    void handleMirrage(PowerCard* power_card_view);
    void handleStorm(PowerCard* power_card_view);
    void handleTide(PowerCard* power_card_view);
    void handleMist(PowerCard* power_card_view);
    void handleWave(PowerCard* power_card_view);
    void handleWhirlpool(PowerCard* power_card_view);
    void handleBlizzard(PowerCard* power_card_view);
    void handleWaterfall(PowerCard* power_card_view);
    void handleSupport(PowerCard* power_card_view);
    void handleEarthquake(PowerCard* power_card_view);
    void handleCrumble(PowerCard* power_card_view);
    void handleBorder(PowerCard* power_card_view);
    void handleAvalanche(PowerCard* power_card_view);
    void handleRock(PowerCard* power_card_view);

private:
    base::ElementalService& m_service;
    GameView* m_view;
};