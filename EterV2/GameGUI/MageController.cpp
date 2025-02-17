#include "MageController.h"

MageController::MageController(QObject* parent, base::MageService& service, GameView* view) : QObject{ parent },
m_service{ service },
m_view{ view } {

    base::MageTypeAbility red_ability{ m_service.getMageAbility(color::ColorType::RED) };
    base::MageTypeAbility blue_ability{ m_service.getMageAbility(color::ColorType::BLUE) };

    QString red_description{ QString(base::mapMageTypeAbilities.at(red_ability).data()) };
    QString blue_description{ QString(base::mapMageTypeAbilities.at(blue_ability).data()) };

    view->drawMages(
        red_ability, red_description,
        blue_ability, blue_description
    );

    connect(view, &GameView::applyMageCard, this, &MageController::onMageCardApply);
}

void MageController::onMageCardApply(MageCard* card) {
    card->setUsed(true);

    selectMage(card);
}

void MageController::selectMage(MageCard* card) {
    std::vector<std::function<void(color::ColorType)>> choices{
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
    choices[index](card->getColor());
}

void MageController::handleFireFront(color::ColorType color) {
    base::MasterOfFireFront* mage_card = m_service.getMageCard<base::MasterOfFireFront>(color);

    auto choices{ mage_card->getChoices() };

    m_view->highlightCards(choices, CardState::ABOUT_TO_REMOVE, CardState::RESTRICTED);
    m_view->hideAllAvailableSpaces();

    connect(m_view, &GameView::clickedOnRemove, this, [this, mage_card](Card* card) {
        mage_card->setSelectedCoord(gui::utils::qPointFToCoord(card->pos()));
        mage_card->apply();

        m_view->resetCardHighlight();
        m_view->showAllAvailableSpaces();

        emit updateCards();
        emit switchPlayer();

        disconnect(m_view, &GameView::clickedOnRemove, this, nullptr);
        });
}
void MageController::handleFireBack(color::ColorType color) {
    base::MasterOfFireBack* mage_card = m_service.getMageCard<base::MasterOfFireBack>(color);

    auto choices{ mage_card->getChoices() };

    m_view->highlightCards(choices, CardState::ABOUT_TO_REMOVE, CardState::RESTRICTED);
    m_view->hideAllAvailableSpaces();

    auto onHoverRemoveEnter = [this, mage_card](Card* card) {
        auto remove_choices{ mage_card->getRemoveChoices(gui::utils::qPointFToCoord(card->pos())) };
        auto card_views{ m_view->getAllCards() };

        for (uint16_t id : remove_choices) {
            card_views[id]->setState(CardState::REMOVE);
        }
        };

    auto onHoverRemoveLeave = [this, mage_card](Card* card) {
        auto remove_choices{ mage_card->getRemoveChoices(gui::utils::qPointFToCoord(card->pos())) };
        auto card_views{ m_view->getAllCards() };

        for (uint16_t id : remove_choices) {
            card_views[id]->setState(CardState::ABOUT_TO_REMOVE);
        }
        };

    auto onClickRemove = [this, mage_card](Card* card) {
        mage_card->setSelectedChoice(gui::utils::qPointFToCoord(card->pos()));
        mage_card->apply();

        m_view->resetCardHighlight();
        m_view->showAllAvailableSpaces();

        for (Card* card : m_view->getAllCards()) {
            disconnect(card, &Card::hoverRemoveEnter, this, nullptr);
            disconnect(card, &Card::hoverRemoveLeave, this, nullptr);
        }

        disconnect(m_view, &GameView::clickedOnRemove, this, nullptr);

        emit updateCards();
        emit switchPlayer();
        };

    for (Card* card : m_view->getAllCards()) {
        connect(card, &Card::hoverRemoveEnter, this, onHoverRemoveEnter);
        connect(card, &Card::hoverRemoveLeave, this, onHoverRemoveLeave);
        connect(m_view, &GameView::clickedOnRemove, this, onClickRemove);
    }

}

void MageController::handleEarthFront(color::ColorType color) {
    base::MasterOfEarthFront* mage_card = m_service.getMageCard<base::MasterOfEarthFront>(color);

    auto cardAppend = [this, mage_card](Card* card) {
        mage_card->setSelectedCoord(gui::utils::qPointFToCoord(card->pos()));
        mage_card->setSelectedCard(card->getID());

        if (mage_card->apply()) {
            card->setPlaced(true);

            m_view->connectCards();

            emit updateCards();
            emit switchPlayer();
        }
        else {
            card->moveCardBack();
        }
        };

    auto cardStartMoving = [this, mage_card](Card* card) {
        auto choices{ mage_card->getChoices(card->getType()) };

        m_view->highlightCards(choices, CardState::AVAILABLE, CardState::RESTRICTED);

        };

    auto cardStoppedMoving = [this](Card* card) {
        m_view->resetCardHighlight();
        };

    for (Card* card : m_view->getAllCards()) {
        card->disconnect();

        connect(card, &Card::cardAppend, this, cardAppend);
        connect(card, &Card::cardStartMoving, this, cardStartMoving);
        connect(card, &Card::cardStoppedMoving, this, cardStoppedMoving);
    }
}
void MageController::handleEarthBack(color::ColorType color) {
    base::MasterOfEarthBack* mage_card = m_service.getMageCard<base::MasterOfEarthBack>(color);

    m_view->highlightCards({}, CardState::RESTRICTED, CardState::RESTRICTED);

    Hole* hole = new Hole({ WINDOW_WIDTH / 2, 700 });
    hole->setFlag(QGraphicsItem::ItemIsMovable, true);
    hole->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_view->scene->addItem(hole);

    connect(hole, &Hole::placed, this, [this, hole, mage_card]() {
        disconnect(hole, &Hole::placed, this, nullptr);

        mage_card->setSelectedCoord(gui::utils::qPointFToCoord(hole->pos()));
        mage_card->apply();

        hole->deleteLater();
        m_view->resetCardHighlight();

        emit updateCards();
        emit switchPlayer();
        });
}

void MageController::handleAirFront(color::ColorType color) {
    base::MasterOfAirFront* mage_card = m_service.getMageCard<base::MasterOfAirFront>(color);
    auto choices{ mage_card->getAvailableChoicesFrom() };
    m_view->highlightCards(choices, CardState::AVAILABLE, CardState::RESTRICTED);
    m_view->makeCardsMovable(choices, true);
    m_view->hideAllAvailableSpaces();

    auto cardAppend = [this, mage_card, choices](Card* card) {
        mage_card->setMoveTo(gui::utils::qPointFToCoord(card->pos()));

        if (mage_card->apply()) {
            m_view->resetCardHighlight();
            m_view->showAllAvailableSpaces();
            m_view->connectCards();
            m_view->makeCardsMovable(choices, false);

            emit updateCards();
            emit switchPlayer();
        }
        else {
            m_view->moveCards(mage_card->m_stackBufer, mage_card->m_moveFrom);
            m_view->highlightCards(choices, CardState::AVAILABLE, CardState::RESTRICTED);
            m_view->hideAllAvailableSpaces();
        }

        m_view->setStackMoveEvent(mage_card->m_stackBufer, false);
        };

    auto cardStartMoving = [this, mage_card, choices](Card* card) {
        if (!choices.contains(card->getID())) {
            return;
        }

        mage_card->setMoveFrom(gui::utils::qPointFToCoord(card->pos()));

        auto choices{ mage_card->getAvailableChoicesTo() };

        m_view->setStackMoveEvent(mage_card->m_stackBufer, true);
        m_view->highlightCards(mage_card->m_stackBufer, CardState::DEFAULT, CardState::RESTRICTED);
        m_view->showAvailableSpaces(mage_card->getAvailableChoicesTo());
        };

    auto cardIsMoving = [this, mage_card](Card* top_card) {
        for (Card* card : m_view->getAllCards()) {
            if (mage_card->m_stackBufer.contains(card->getID())) {
                card->setPos(top_card->pos());
            }
        }
        };

    auto cardStoppedMoving = [this, mage_card](Card* top_card) {
        for (Card* card : m_view->getAllCards()) {
            if (mage_card->m_stackBufer.contains(card->getID())) {
                card->setPos(top_card->pos());
            }
        }
        };

    for (Card* card : m_view->getAllCards()) {
        card->disconnect();

        connect(card, &Card::cardAppend, this, cardAppend);
        connect(card, &Card::cardStartMoving, this, cardStartMoving);
        connect(card, &Card::isMoving, this, cardIsMoving);
        connect(card, &Card::cardStoppedMoving, this, cardStoppedMoving);
    }
}
void MageController::handleAirBack(color::ColorType color) {
    base::MasterOfAirBack* mage_card = m_service.getMageCard<base::MasterOfAirBack>(color);
    uint16_t id{ mage_card->createEter() };

    m_view->highlightCards({}, CardState::RESTRICTED, CardState::RESTRICTED);

    Card* eter_view = new Card(color, base::CombatCardType::ETER, { WINDOW_WIDTH / 2, 700 }, id);

    m_view->scene->addItem(eter_view);
    m_view->getAllCards().insert(id, eter_view);

    connect(eter_view, &Card::cardAppend, this, [this](Card* card) {
        m_view->resetCardHighlight();

        emit cardAppend(card);
        });
}

void MageController::handleWaterFront(color::ColorType color) {
    base::MasterOfWaterFront* mage_card = m_service.getMageCard<base::MasterOfWaterFront>(color);
    auto choices{ mage_card->getAvailableChoicesFrom() };
    m_view->highlightCards(choices, CardState::AVAILABLE, CardState::RESTRICTED);
    m_view->makeCardsMovable(choices, true);
    m_view->hideAllAvailableSpaces();

    auto cardAppend = [this, mage_card, choices](Card* card) {
        mage_card->setMoveTo(gui::utils::qPointFToCoord(card->pos()));

        if (mage_card->apply()) {
            m_view->resetCardHighlight();
            m_view->showAllAvailableSpaces();
            m_view->connectCards();
            m_view->makeCardsMovable(choices, false);

            emit updateCards();
            emit switchPlayer();
        }
        else {
            m_view->moveCards(mage_card->m_stackBufer, mage_card->m_moveFrom);
            m_view->highlightCards(choices, CardState::AVAILABLE, CardState::RESTRICTED);
            m_view->hideAllAvailableSpaces();
        }

        m_view->setStackMoveEvent(mage_card->m_stackBufer, false);
        };

    auto cardStartMoving = [this, mage_card, choices](Card* card) {
        if (!choices.contains(card->getID())) {
            return;
        }

        mage_card->setMoveFrom(gui::utils::qPointFToCoord(card->pos()));

        auto choices{ mage_card->getAvailableChoicesTo() };

        m_view->setStackMoveEvent(mage_card->m_stackBufer, true);
        m_view->highlightCards(mage_card->m_stackBufer, CardState::DEFAULT, CardState::RESTRICTED);
        m_view->showAvailableSpaces(mage_card->getAvailableChoicesTo());
        };

    auto cardIsMoving = [this, mage_card](Card* top_card) {
        for (Card* card : m_view->getAllCards()) {
            if (mage_card->m_stackBufer.contains(card->getID())) {
                card->setPos(top_card->pos());
            }
        }
        };

    auto cardStoppedMoving = [this, mage_card](Card* top_card) {
        for (Card* card : m_view->getAllCards()) {
            if (mage_card->m_stackBufer.contains(card->getID())) {
                card->setPos(top_card->pos());
            }
        }
        };

    for (Card* card : m_view->getAllCards()) {
        card->disconnect();

        connect(card, &Card::cardAppend, this, cardAppend);
        connect(card, &Card::cardStartMoving, this, cardStartMoving);
        connect(card, &Card::isMoving, this, cardIsMoving);
        connect(card, &Card::cardStoppedMoving, this, cardStoppedMoving);
    }
}
void MageController::handleWaterBack(color::ColorType color) {
    base::MasterOfWaterBack* mage_card = m_service.getMageCard<base::MasterOfWaterBack>(color);
    auto choices{ mage_card->getChoices() };
    m_view->highlightCards(choices, CardState::AVAILABLE, CardState::RESTRICTED);
    m_view->makeCardsMovable(choices, true);
    m_view->hideAllAvailableSpaces();

    auto cardAppend = [this, mage_card](Card* card) {
        mage_card->setEnd(gui::utils::qPointFToCoord(card->pos()));
        auto card_views{ m_view->getAllCards() };

        if (mage_card->apply()) {
            m_view->resetCardHighlight();
            m_view->showAllAvailableSpaces();
            m_view->connectCards();

            emit updateCards();
            emit switchPlayer();
        }
        else {
            for (const auto& [coord, ids] : mage_card->moving_cards_buffer) {
                card_views[ids.back()]->setPos(gui::utils::coordToQPointF(coord));
            }

            mage_card->resetBuffer();
            m_view->hideAllAvailableSpaces();
        }

        m_view->showCards([](Card* card) { return card->isPlaced();  });
        };

    auto cardStartMoving = [this, mage_card, choices](Card* card) {
        mage_card->setStart(gui::utils::qPointFToCoord(card->pos()));

        m_view->drawAvailableSpaces(mage_card->getAvailableSpaces());

        auto card_views{ m_view->getAllCards() };
        for (const auto& [coord, ids] : mage_card->moving_cards_buffer) {
            for (uint16_t id : ids) {
                if (id != ids.back()) {
                    card_views[id]->hide();
                }
            }
        }
        };

    auto cardIsMoving = [this, mage_card](Card* top_card, QPointF delta) {
        auto card_views{ m_view->getAllCards() };

        for (const auto& [coord, ids] : mage_card->moving_cards_buffer) {
            if (ids.back() != top_card->getID()) {
                card_views[ids.back()]->setPos(card_views[ids.back()]->pos() + delta);
            }
        }
        };

    for (Card* card : m_view->getAllCards()) {
        card->disconnect();

        connect(card, &Card::cardAppend, this, cardAppend);
        connect(card, &Card::cardStartMoving, this, cardStartMoving);
        connect(card, &Card::movingWithDelta, this, cardIsMoving);
    }
}