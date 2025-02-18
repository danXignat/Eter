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
    std::vector<std::function<void(uint16_t)>> choices{
        [this](uint16_t id) { this->handleControllerExplosion(id); },
        [this](uint16_t id) { this->handleDestruction(id); },
        [this](uint16_t id) { this->handleFlame(id); },
        [this](uint16_t id) { this->handleFire(id); },
        [this](uint16_t id) { this->handleAsh(id); },
        [this](uint16_t id) { this->handleSpark(id); },
        [this](uint16_t id) { this->handleSquall(id); },
        [this](uint16_t id) { this->handleGale(id); },
        [this](uint16_t id) { this->handleHurricane(id); },
        [this](uint16_t id) { this->handleGust(id); },
        [this](uint16_t id) { this->handleMirrage(id); },
        [this](uint16_t id) { this->handleStorm(id); },
        [this](uint16_t id) { this->handleTide(id); },
        [this](uint16_t id) { this->handleMist(id); },
        [this](uint16_t id) { this->handleWave(id); },
        [this](uint16_t id) { this->handleWhirlpool(id); },
        [this](uint16_t id) { this->handleBlizzard(id); },
        [this](uint16_t id) { this->handleWaterfall(id); },
        [this](uint16_t id) { this->handleSupport(id); },
        [this](uint16_t id) { this->handleEarthquake(id); },
        [this](uint16_t id) { this->handleCrumble(id); },
        [this](uint16_t id) { this->handleBorder(id); },
        [this](uint16_t id) { this->handleAvalanche(id); },
        [this](uint16_t id) { this->handleRock(id); }
    };

    int index{ static_cast<uint16_t>(card->getTypeAbility()) };
    choices[index](card->getId());
}

void ElementalController::handleControllerExplosion(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::ControllerExplosion>(id);

    if (power_card->apply()) {
        m_view->drawExplosion(power_card->getExplosionService(), power_card->getExplosionService().size(), EXPLOSION_POS);
    }
    else{

    }
}

void ElementalController::handleDestruction(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Destruction>(id);
}

void ElementalController::handleFlame(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Flame>(id);
}

void ElementalController::handleFire(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Fire>(id);
}

void ElementalController::handleAsh(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Ash>(id);
}

void ElementalController::handleSpark(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Spark>(id);
}

void ElementalController::handleSquall(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Squall>(id);
}

void ElementalController::handleGale(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Gale>(id);
}

void ElementalController::handleHurricane(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Hurricane>(id);
}

void ElementalController::handleGust(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Gust>(id);
}

void ElementalController::handleMirrage(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Mirrage>(id);
}

void ElementalController::handleStorm(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Storm>(id);
}

void ElementalController::handleTide(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Tide>(id);
}

void ElementalController::handleMist(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Mist>(id);
}

void ElementalController::handleWave(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Wave>(id);
}

void ElementalController::handleWhirlpool(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Whirlpool>(id);
}

void ElementalController::handleBlizzard(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Blizzard>(id);
}

void ElementalController::handleWaterfall(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Waterfall>(id);
}

void ElementalController::handleSupport(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Support>(id);
}

void ElementalController::handleEarthquake(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Earthquake>(id);
}

void ElementalController::handleCrumble(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Crumble>(id);
}

void ElementalController::handleBorder(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Border>(id);
}

void ElementalController::handleAvalanche(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Avalanche>(id);
}

void ElementalController::handleRock(uint16_t id) {
    auto* power_card = m_service.getElementalCard<base::Rock>(id);
}