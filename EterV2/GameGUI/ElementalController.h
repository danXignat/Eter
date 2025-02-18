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

public:
    ElementalController(QObject* parent, base::ElementalService& service, GameView* view);

    void selectPower(PowerCard* card);
    void onPowerCardApply(PowerCard* card);

    void handleControllerExplosion(uint16_t id);
    void handleDestruction(uint16_t id);
    void handleFlame(uint16_t id);
    void handleFire(uint16_t id);
    void handleAsh(uint16_t id);
    void handleSpark(uint16_t id);
    void handleSquall(uint16_t id);
    void handleGale(uint16_t id);
    void handleHurricane(uint16_t id);
    void handleGust(uint16_t id);
    void handleMirrage(uint16_t id);
    void handleStorm(uint16_t id);
    void handleTide(uint16_t id);
    void handleMist(uint16_t id);
    void handleWave(uint16_t id);
    void handleWhirlpool(uint16_t id);
    void handleBlizzard(uint16_t id);
    void handleWaterfall(uint16_t id);
    void handleSupport(uint16_t id);
    void handleEarthquake(uint16_t id);
    void handleCrumble(uint16_t id);
    void handleBorder(uint16_t id);
    void handleAvalanche(uint16_t id);
    void handleRock(uint16_t id);
private:
    base::ElementalService& m_service;
    GameView* m_view;
};