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

    auto cardAppend = [this, power_card](Card* card) {
        if (power_card->placeCard(gui::utils::qPointFToCoord(card->pos()), card->getID())) {
            card->setPlaced(true);

            m_view->resetCardHighlight();
            m_view->connectCards();

            emit updateCards();
            emit switchPlayer();
        }
        else {
            card->moveCardBack();
        }
    };

    for (Card* card : m_view->getAllCards()) {
        card->disconnect();

        connect(card, &Card::cardAppend, this, cardAppend);
    }

    if (power_card->apply()) {
        auto card_views{ m_view->getAllCards() };

        uint16_t flipped_card_id{power_card->getAppliedId().value()};

        card_views[flipped_card_id]->flipCard();

        auto choices{ power_card->getCardChoices() };
        m_view->highlightCards(choices, CardState::AVAILABLE, CardState::RESTRICTED);
    }
    else {
        power_card_view->goBack();
        power_card_view->show();

        m_view->connectCards();
    }
}

void ElementalController::handleFire(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Fire>(power_card_view->getId());
    auto choices{ power_card->getVisibleCardIDs() };

    m_view->highlightCards(choices, CardState::ABOUT_TO_HAND, CardState::RESTRICTED);
    m_view->hideAllAvailableSpaces();

    auto onHoverRemoveEnter = [this, power_card](Card* card) {
        auto remove_choices{ power_card->getCardIDsOfType(card->getType()) };
        auto card_views{ m_view->getAllCards() };

        for (uint16_t id : remove_choices) {
            card_views[id]->setState(CardState::HAND);
        }
        };

    auto onHoverRemoveLeave = [this, power_card](Card* card) {
        auto remove_choices{ power_card->getCardIDsOfType(card->getType()) };
        auto card_views{ m_view->getAllCards() };

        for (uint16_t id : remove_choices) {
            card_views[id]->setState(CardState::ABOUT_TO_HAND);
        }
        };

    auto onClickRemove = [this, power_card](Card* card) {
        power_card->setChosenCard(card->getType());

        if (power_card->apply()) {
            m_view->resetCardHighlight();
            m_view->showAllAvailableSpaces();
            m_view->connectCards();

            emit updateCards();
            emit switchPlayer();
        }
        };

    for (Card* card : m_view->getAllCards()) {
        card->disconnect();

        connect(card, &Card::hoverRemoveEnter, this, onHoverRemoveEnter);
        connect(card, &Card::hoverRemoveLeave, this, onHoverRemoveLeave);
        connect(card, &Card::clickedOnRemove, this, onClickRemove);
    }
}

void ElementalController::handleAsh(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Ash>(power_card_view->getId());

    color::ColorType color{ color::ColorType::DEFAULT };
    emit getCurrPlayer(color);
    power_card->setColor(color);

    auto card_views{ m_view->getAllCards() };
    auto choices{ power_card->getUsedCardIDs(color) };

    int x{ WINDOW_WIDTH / 2 - (int(choices.size()) - 1) * (CARD_SIZE + 10) / 2};
    for (uint16_t id : choices) {
        card_views[id]->show();
        card_views[id]->setPos(x, 700);
        x += CARD_SIZE + 10;
    }

    auto localCardAppend = [this, power_card](Card* card) {
        power_card->setSelectionByID(gui::utils::qPointFToCoord(card->pos()), card->getID());
        power_card->setType(card->getType());

        if (power_card->apply()) {
            card->setPlaced(true);
            m_view->connectCards();

            emit updateCards();
            emit switchPlayer();
        }
        else {
            card->moveCardBack();
        }
        };

    for (Card* card : m_view->getAllCards()) {
        disconnect(card, &Card::cardAppend, nullptr, nullptr);

        connect(card, &Card::cardAppend, this, localCardAppend);
    }
}

void ElementalController::handleSpark(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Spark>(power_card_view->getId());

    color::ColorType color{ color::ColorType::DEFAULT };
    emit getCurrPlayer(color);
    power_card->setColor(color);

    auto card_views{ m_view->getAllCards() };
    auto choices{ power_card->getCoveredCardIDs() };

    int x{ WINDOW_WIDTH / 2 - (int(choices.size()) - 1) * (CARD_SIZE + 10) / 2 };
    for (uint16_t id : choices) {
        card_views[id]->show();
        card_views[id]->setPlaced(false);
        card_views[id]->setPos(x, 700);
        card_views[id]->saved_pos = card_views[id]->pos();
        x += CARD_SIZE + 10;
    }

    auto localCardAppend = [this, power_card, choices](Card* card) {
        power_card->setSelectionByID(card->getID());
        power_card->setMoveDestination(gui::utils::qPointFToCoord(card->pos()));
        power_card->setId(card->getID());

        if (power_card->apply()) {
            card->setPlaced(true);
            m_view->connectCards();

            emit updateCards();
            emit switchPlayer();
        }
        else {
            card->setPos(card->saved_pos);
        }
        };

    for (Card* card : m_view->getAllCards()) {
        disconnect(card, &Card::cardAppend, nullptr, nullptr);

        connect(card, &Card::cardAppend, this, localCardAppend);
    }
}

void ElementalController::handleSquall(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Squall>(power_card_view->getId());

    color::ColorType color{ color::ColorType::DEFAULT };
    emit getCurrPlayer(color);
    power_card->setColor(color);

    auto choices{ power_card->getVisibleCardsIDs() };

    m_view->highlightCards(choices, CardState::ABOUT_TO_HAND, CardState::RESTRICTED);
    m_view->hideAllAvailableSpaces();

    auto onHoverRemoveEnter = [this, power_card](Card* card) {
        
        };

    auto onHoverRemoveLeave = [this, power_card](Card* card) {
        
        };

    auto onClickRemove = [this, power_card](Card* card) {
        power_card->setSelectedCardID(card->getID());
        power_card->setSelectedCardCoord(gui::utils::qPointFToCoord(card->pos()));

        if (power_card->apply()) {
            m_view->resetCardHighlight();
            m_view->showAllAvailableSpaces();
            m_view->connectCards();

            emit updateCards();
            emit switchPlayer();
        }
        };

    for (Card* card : m_view->getAllCards()) {
        card->disconnect();

        connect(card, &Card::hoverRemoveEnter, this, onHoverRemoveEnter);
        connect(card, &Card::hoverRemoveLeave, this, onHoverRemoveLeave);
        connect(card, &Card::clickedOnRemove, this, onClickRemove);
    }
}

void ElementalController::handleGale(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Gale>(power_card_view->getId());

    if (power_card->apply()) {
        emit updateCards();
        emit switchPlayer();
    }
    else {
        power_card_view->goBack();
        power_card_view->show();
    }
}

void ElementalController::handleHurricane(PowerCard* power_card_view) {
    auto* power_card = m_service.getElementalCard<base::Hurricane>(power_card_view->getId());
    auto card_views{ m_view->getAllCards() };
    for (auto* card : card_views) {
        card->setState(CardState::ABOUT_TO_CHECK);
    }
    
    auto onHoverRemoveEnter = [this, power_card](Card* card) {

        };

    auto onHoverRemoveLeave = [this, power_card](Card* card) {

        };

    auto onClickRemove = [this, power_card](Card* card) {
        

        if (power_card->apply()) {
            m_view->resetCardHighlight();
            m_view->showAllAvailableSpaces();
            m_view->connectCards();

            emit updateCards();
            emit switchPlayer();
        }
        };

    for (Card* card : m_view->getAllCards()) {
        card->disconnect();

        connect(card, &Card::hoverRemoveLeave, this, onHoverRemoveLeave);
        connect(card, &Card::clickedOnRemove, this, onClickRemove);
    }
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