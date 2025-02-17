#include "GameController.h"
#include <functional>
#include <ranges>

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

        connect(card, &Card::cardAppend,        this, cardAppend);
        connect(card, &Card::cardStartMoving,   this, cardStartMoving);
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

///------------------------
///---------------

GameController::GameController(
    QWidget* parent,
    const std::string& mode,
    const QString& name_red, const QString& name_blue
) : QObject{ parent },
    model{ base::GameModeFactory::get(mode, {name_red.toStdString(), name_blue.toStdString()}) },
    view{ new GameView(name_red, name_blue, parent) },
    m_mage_controller{ nullptr } {

    if (auto* mode = dynamic_cast<base::MageMode*>(model.get())) {
        m_mage_controller = new MageController(this, mode->getMageService(), view);
    }

    _initConections();
    _initVisuals();
    
    view->switchToPlayer(model->getCurrPlayer());
}

///--------------------------------------------------------------------------SIGNALS------------------------------------------------------

void GameController::onCardAppend(Card* card) {
    base::InputHandler input;
    input.event_type = card->isIllusion() ? base::EventType::PlaceIllusion : base::EventType::PlaceCombatCard;
    input.card_type = card->getType();
    input.coord = gui::utils::qPointFToCoord(card->pos());
    input.ID = card->getID();

    if (card->isIllusion() == false && !model->placeCombatCard(input)) {
        card->moveCardBack();
    }
    else if (card->isIllusion() == true && 
        model->getIllusionService().has_value() && !model->getIllusionService()->placeIllusion(input)) {
        card->moveCardBack();
    }
    else {
        bool change_player = true;

        card->setPlaced(true);
        view->drawAvailablePositions(model->getBoard());

        if (auto win_color = model->getWinningColor(); win_color.has_value()) {
            view->showWin(win_color.value());
        }

        if (model->getExplosionService().has_value() &&
            model->getExplosionService()->checkAvailability()
            ) {

            for (const auto& [id, card] : view->getAllCards().asKeyValueRange()) {
                card->setFlag(QGraphicsItem::ItemIsMovable, false);
            }

            view->setExplosionActive();
            _updateTargetZone();
            change_player = false;
        }

        if (change_player) {
            _updateBoardView();
            model->switchPlayer();
            view->switchToPlayer(model->getCurrPlayer());
        }

    }

}

void GameController::onExplosionApply() {
    model->getExplosionService()->apply();
    this->_updateBoardView();
    model->removeExplosion();
    model->switchPlayer();
    view->drawAvailablePositions(model->getBoard());
    view->eraseExplosion();
    view->switchToPlayer(model->getCurrPlayer());
}

void GameController::onExplosionDiscard() {
    model->switchPlayer();
    model->removeExplosion();
    view->eraseExplosion();
    view->switchToPlayer(model->getCurrPlayer());
}

void GameController::onExplosionRotateLeft() {
    model->getExplosionService()->rotateLeft();
}

void GameController::onExplosionRotateRight() {
    model->getExplosionService()->rotateRight();
}

void GameController::onSelectIllusion(Card* card) {
    color::ColorType color{ card->getColor() };

    bool has_illusion{ model->getIllusionService().has_value() && model->getIllusionService()->hasIllusion(color) };
    bool& illusion_in_use{ (color == color::ColorType::RED) ? view->red_illusion_in_use : view->blue_illusion_in_use };

    if (has_illusion && !illusion_in_use) {
        card->flipCard();
        illusion_in_use = true;
    }
    else if (has_illusion && card->isIllusion()) {
        card->flipCard();
        illusion_in_use = false;
    }
}

void GameController::onCardStartMoving(Card* card) {
    if (card->isPlaced()) {
        return;
    }

    base::InputHandler input;
    input.color = card->getColor();
    input.card_type = card->getType();
    input.event_type = (card->isIllusion() ? base::EventType::PlaceIllusion : base::EventType::PlaceCombatCard);

    auto available_coords { model->getAvailableIds(input) };
    auto restricted_coords{ model->getRestrictedIds(input) };

    for (auto [id, card_view] : view->getAllCards().asKeyValueRange()) {
        bool valid_change{ card_view->isPlaced() && id != card->getID() };

        if (valid_change && available_coords.contains(id)) {
            card_view->setState(CardState::AVAILABLE);
        }
        else if (valid_change && restricted_coords.contains(id)) {
            card_view->setState(CardState::RESTRICTED);
        }
    }
}

void GameController::onCardStoppedMoving(Card* card) {
    for (Card* card_view : view->getAllCards()) {
        card_view->setState(CardState::DEFAULT);
    }
}

void GameController::onSwitchPlayer() {
    model->switchPlayer();
    view->switchToPlayer(model->getCurrPlayer());
}

void GameController::onUpdateCards() {
    _updateBoardView();
}

///--------------------------------------------------------INIT-------------------------------------------------

void GameController::_initConections() {
    connect(view, &GameView::cardAppend,            this, &GameController::onCardAppend);
    connect(view, &GameView::cardStartMoving,       this, &GameController::onCardStartMoving);
    connect(view, &GameView::cardStoppedMoving,     this, &GameController::onCardStoppedMoving);
    connect(view, &GameView::selectIllusion,        this, &GameController::onSelectIllusion);

    connect(view, &GameView::explosionApply,        this, &GameController::onExplosionApply);
    connect(view, &GameView::explosionDiscard,      this, &GameController::onExplosionDiscard);
    connect(view, &GameView::explosionRotateLeft,   this, &GameController::onExplosionRotateLeft);
    connect(view, &GameView::explosionRotateRight,  this, &GameController::onExplosionRotateRight);

    if (m_mage_controller) {
        connect(m_mage_controller, &MageController::switchPlayer, this, &GameController::onSwitchPlayer);
        connect(m_mage_controller, &MageController::updateCards, this, &GameController::onUpdateCards);
        connect(m_mage_controller, &MageController::cardAppend, this, &GameController::onCardAppend);
    }
}

void GameController::_initVisuals() {
    qreal cardStartPos;
    if (dynamic_cast<base::TrainingMode*>(model.get())) {
        cardStartPos = 360;
    }
    else {
        cardStartPos = 240;
    }

    view->drawPlayerCards(model->getPlayerRed(), { cardStartPos, 800 });
    view->drawPlayerCards(model->getPlayerBlue(), { cardStartPos, 800 });
    view->drawAvailablePositions(model->getBoard());

    if (model->getExplosionService().has_value()) {
        view->drawExplosion(*model->getExplosionService(), model->getBoard().size(), EXPLOSION_POS);
    }

    view->switchToPlayer(model->getCurrPlayer());
}

///----------------------------------------------------------UPDATE-------------------------------------------------

void GameController::_updateBoardView() {
    auto& card_views = view->getAllCards();
    const auto& curr_player_model = model->getCurrPlayer();
    const auto& player_red_model = model->getPlayerRed();
    const auto& player_blue_model = model->getPlayerBlue();
    const auto& board_model = model->getBoard();

    bool hide_red{ (curr_player_model.getColor() == color::ColorType::RED) ? false : true };

    view->drawAvailablePositions(board_model);

    _updateTargetZone();
    _updatePlayerCards(player_red_model, card_views, !hide_red);
    _updatePlayerCards(player_blue_model, card_views, hide_red);
    _updateBoardCards(board_model, card_views);
}

void GameController::_updatePlayerCards(const base::Player& player, QHash<uint16_t, Card*>& card_views, bool hide) {
    for (const auto& [type, card] : player.getCards()) {
        if (card.isCombatCard() == false) {
            continue;
        }

        uint16_t ID{ card.getID() };
        card_views[ID]->moveCardBack();
        card_views[ID]->setFlag(QGraphicsItem::ItemIsMovable);
        card_views[ID]->update();

        if (hide) {
            card_views[ID]->hide();
            card_views[ID]->setVisible(false);
            card_views[ID]->setPlaced(false);
        }
    }

    for (const auto& [type, card] : player.getUsedCards()) {
        if (card.isCombatCard() == false) {
            continue;
        }

        uint16_t ID{ card.getID() };

        card_views[ID]->moveCardBack();

        card_views[ID]->hide();
        card_views[ID]->setPlaced(false);
        card_views[ID]->setUsed(true);
    }
}

void GameController::_updateBoardCards(const base::Board& board, QHash<uint16_t, Card*>& card_views) {
    for (const auto& [coord, stack] : board) {
        uint16_t first_card_id{ stack.front().getID() };

        if (stack.front().getType() == base::CombatCardType::HOLE) {
            view->drawHole(gui::utils::coordToQPointF(coord));
        }
        else if(card_views[first_card_id]->isIllusion() == true && stack.front().isIllusion() == false){
            card_views[first_card_id]->flipCard();
        }
    }

    for (const auto& [coord, stack] : board) {
        for (const base::CombatCard& card : stack) {
            uint16_t id{ card.getID() };
            QPointF pos{ gui::utils::coordToQPointF(coord) };

            if (card_views.contains(id) && pos != card_views[id]->pos()) {
                card_views[id]->setPos(pos);
            }
        }
    }
}

void GameController::_updateTargetZone() {
    TargetZone* zone{ view->getTargetZone() };

    auto [coord1, coord2] { model->getBoard().getBoudingRect() };

    zone->setCorners(gui::utils::coordToQPointF(coord1), gui::utils::coordToQPointF(coord2));
}