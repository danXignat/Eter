#include "ElementalController.h"

ElementalController::ElementalController(QObject* parent, base::ElementalService& service, GameView* view) 
    : QObject{ parent },
    m_service{ service },
    m_view{ view } {

    view->drawPowers(service.getCardsData());

    connect(view, &GameView::applyPowerCard, this, &ElementalController::onPowerCardApply);
}

void ElementalController::onPowerCardApply(PowerCard* card) {
    card->setUsed(true);

    selectPower(card);
}

void ElementalController::selectPower(PowerCard* card) {
    /*std::vector<std::function<void(color::ColorType)>> choices{
        [this](color::ColorType color) { this->handleFireFront(color); },
        [this](color::ColorType color) { this->handleFireBack(color); },
        [this](color::ColorType color) { this->handleEarthFront(color); },
        [this](color::ColorType color) { this->handleEarthBack(color); },
        [this](color::ColorType color) { this->handleAirFront(color); },
        [this](color::ColorType color) { this->handleAirBack(color); },
        [this](color::ColorType color) { this->handleWaterFront(color); },
        [this](color::ColorType color) { this->handleWaterBack(color); }
    };

    int index{ static_cast<int>(card->getTypeAbility()) };
    choices[index](card->getColor());*/
}