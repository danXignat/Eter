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