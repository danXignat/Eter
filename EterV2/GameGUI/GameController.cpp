#include "GameController.h"
#include <functional>
#include <ranges>

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

    if (auto* mode = dynamic_cast<base::ElementalMode*>(model.get())) {
        m_elemental_controller = new ElementalController(this, mode->getElementalService(), view);
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

    if (dynamic_cast<base::TournamentMode*>(model.get())) {
        view->drawArena(model->getGameSizeType());
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