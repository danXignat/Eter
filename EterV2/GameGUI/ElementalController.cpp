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
    std::vector<std::function<void(PowerCard*)>> choices{
        [this](PowerCard* power_card_view) { this->handleControllerExplosion(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleDestruction(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleFlame(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleFire(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleAsh(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleSpark(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleSquall(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleGale(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleHurricane(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleGust(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleMirrage(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleStorm(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleTide(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleMist(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleWave(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleWhirlpool(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleBlizzard(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleWaterfall(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleSupport(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleEarthquake(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleCrumble(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleBorder(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleAvalanche(power_card_view); },
        [this](PowerCard* power_card_view) { this->handleRock(power_card_view); }
    };

    int index{ static_cast<uint16_t>(card->getTypeAbility()) };
    choices[index](card);
}

void ElementalController::handleControllerExplosion(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::ControllerExplosion>(power_card_view->getId());


    if (power_card->apply()) {
        emit addExplosion();
        m_view->setExplosionActive();
    }
    else{
        power_card_view->goBack();
        power_card_view->show();
    }
}

void ElementalController::handleDestruction(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Destruction>(power_card_view->getId());

    color::ColorType color{color::ColorType::DEFAULT};

    emit getCurrPlayer(color);

    power_card->setColor(color);

    if (!power_card->apply()) {
        power_card_view->goBack();
        power_card_view->show();
    }

    emit updateCards();
    emit switchPlayer();
}

void ElementalController::handleFlame(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Flame>(power_card_view->getId());

    color::ColorType color{ color::ColorType::DEFAULT };

    emit getCurrPlayer(color);

    power_card->setColor(color);
}

void ElementalController::handleFire(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Fire>(power_card_view->getId());
}

void ElementalController::handleAsh(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Ash>(power_card_view->getId());
}

void ElementalController::handleSpark(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Spark>(power_card_view->getId());
}

void ElementalController::handleSquall(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Squall>(power_card_view->getId());
}

void ElementalController::handleGale(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Gale>(power_card_view->getId());
}

void ElementalController::handleHurricane(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Hurricane>(power_card_view->getId());
}

void ElementalController::handleGust(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Gust>(power_card_view->getId());
}

void ElementalController::handleMirrage(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Mirrage>(power_card_view->getId());
}

void ElementalController::handleStorm(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Storm>(power_card_view->getId());
}

void ElementalController::handleTide(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Tide>(power_card_view->getId());
}

void ElementalController::handleMist(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Mist>(power_card_view->getId());
}

void ElementalController::handleWave(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Wave>(power_card_view->getId());
}

void ElementalController::handleWhirlpool(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Whirlpool>(power_card_view->getId());
}

void ElementalController::handleBlizzard(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Blizzard>(power_card_view->getId());
}

void ElementalController::handleWaterfall(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Waterfall>(power_card_view->getId());
}

void ElementalController::handleSupport(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Support>(power_card_view->getId());
}

void ElementalController::handleEarthquake(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Earthquake>(power_card_view->getId());
}

void ElementalController::handleCrumble(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Crumble>(power_card_view->getId());
}

void ElementalController::handleBorder(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Border>(power_card_view->getId());
}

void ElementalController::handleAvalanche(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Avalanche>(power_card_view->getId());
}

void ElementalController::handleRock(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Rock>(power_card_view->getId());
}