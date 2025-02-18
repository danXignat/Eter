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

private:
    base::ElementalService& m_service;
    GameView* m_view;
};