#include "GameController.h"
#include <functional>
#include <ranges>

///------------------------
///---------------

GameController::GameController(
    QWidget* parent,
    const QString& name_red, const QString& name_blue,
    const GameModeConfig& config
) : QObject{ parent },
m_mage_controller{ nullptr },
m_elemental_controller{ nullptr } {

    if (config.id.front() == '5') {
        model = base::GameModeFactory::getTimedMode(config.id, { name_red.toStdString(), name_blue.toStdString() }, *config.time, *config.size_type);
    }
    else {
        model = base::GameModeFactory::get(config.id, { name_red.toStdString(), name_blue.toStdString() });
    }

    view = new GameView(name_red, name_blue, config, parent);

    _initVisuals();
    _initServices();
    _initConections();
    
    view->switchToPlayer(model->getCurrPlayer());
}

///--------------------------------------------------------------------------SIGNALS------------------------------------------------------

void GameController::onCardAppend(Card* card) {
    base::InputHandler input;
    input.event_type = card->isIllusion() ? base::EventType::PlaceIllusion : base::EventType::PlaceCombatCard;
    input.card_type = card->getType();
    input.coord = gui::utils::qPointFToCoord(card->pos());
    input.color = card->getColor();
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

            if (dynamic_cast<base::TimedMode*>(model.get())) {
                view->red_timer->reset();
                view->blue_timer->reset();
            }
            _handleWin(*win_color);
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
            onSwitchPlayer();
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

void GameController::onAddExplosion() {
    model->addExplosion();
    view->drawExplosion(*model->getExplosionService(), model->getBoard().size(), EXPLOSION_POS);
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

    if (auto* mode = dynamic_cast<base::TimedMode*>(model.get())) {
        color::ColorType color{ model->getCurrPlayer().getColor() };

        if (color == color::ColorType::RED) {
            view->blue_timer->pause();
            view->red_timer->start();
        }
        else {
            view->red_timer->pause();
            view->blue_timer->start();
        }
    }
}

void GameController::onUpdateCards() {
    _updateBoardView();
}

void GameController::onNextRound() {
    model->nextRound();
    for (Card* card : view->getAllCards()) {
        card->setPlaced(false);
    }

    _updateBoardView();
    view->switchToPlayer(model->getCurrPlayer());

    if (auto* mode = dynamic_cast<base::TournamentMode*>(model.get())) {
        auto [coord, color] { mode->getArenaService().getLastMarkerAdded()};

        view->drawMarker(gui::utils::coordToQPointF(coord), color);
    }

    if (auto* mode = dynamic_cast<base::TimedMode*>(model.get())) {
        view->red_timer->reset();
        view->blue_timer->reset();

        if (model->getCurrPlayer().getColor() == color::ColorType::RED) {
            view->red_timer->start();
        }
        else {
            view->blue_timer->start();
        }
    }

    if (model->getExplosionService().has_value()) {
        view->eraseExplosion();
        model->removeExplosion();

        onAddExplosion();
    }
}

void GameController::onGettingCurrPlayer(color::ColorType& color) {
    const auto& player_color{ model->getCurrPlayer().getColor() };

    color = player_color;
}

void GameController::onTimedUp(color::ColorType color) {
    color::ColorType other_color{ (color == color::ColorType::RED) ? color::ColorType::BLUE : color::ColorType::RED };

    auto* mode = dynamic_cast<base::TimedMode*>(model.get());

    mode->setLostOnTime(color);

    _handleWin(other_color);
}

void GameController::_handleWin(color::ColorType color) {
    if (model->isFinalRound()) {
        view->victory_screen->switchToMainMenu();
    }

    view->victory_screen->showVictory(color, 
        model->getScore(color::ColorType::RED),
        model->getScore(color::ColorType::BLUE)
    );
}

///--------------------------------------------------------INIT-------------------------------------------------

void GameController::_initServices() {
    auto* mage_mode = dynamic_cast<base::MageMode*>(model.get());
    auto* elemental_mode = dynamic_cast<base::ElementalMode*>(model.get());
    auto* tournament_mode = dynamic_cast<base::TournamentMode*>(model.get());
    auto* timed_mode = dynamic_cast<base::TimedMode*>(model.get());

    if (mage_mode) {
        m_mage_controller = new MageController(this, mage_mode->getMageService(), view);
    }

    ///

    if (elemental_mode) {
        m_elemental_controller = new ElementalController(this, elemental_mode->getElementalService(), view);
    }

    ///

    if (tournament_mode && tournament_mode->getMageService().has_value()) {
        m_mage_controller = new MageController(this, *tournament_mode->getMageService(), view);
    }

    if (tournament_mode && tournament_mode->getElementalService().has_value()) {
        m_elemental_controller = new ElementalController(this, *tournament_mode->getElementalService(), view);
    }

    ///

    if (timed_mode && timed_mode->getMageService().has_value()) {
        m_mage_controller = new MageController(this, *timed_mode->getMageService(), view);
    }

    if (timed_mode && timed_mode->getElementalService().has_value()) {
        m_elemental_controller = new ElementalController(this, *timed_mode->getElementalService(), view);
    }
}

void GameController::_initConections() {
    connect(view, &GameView::cardAppend,            this, &GameController::onCardAppend);
    connect(view, &GameView::cardStartMoving,       this, &GameController::onCardStartMoving);
    connect(view, &GameView::cardStoppedMoving,     this, &GameController::onCardStoppedMoving);
    connect(view, &GameView::selectIllusion,        this, &GameController::onSelectIllusion);

    connect(view, &GameView::explosionApply,        this, &GameController::onExplosionApply);
    connect(view, &GameView::explosionDiscard,      this, &GameController::onExplosionDiscard);
    connect(view, &GameView::explosionRotateLeft,   this, &GameController::onExplosionRotateLeft);
    connect(view, &GameView::explosionRotateRight,  this, &GameController::onExplosionRotateRight);

    connect(view, &GameView::mainMenuRequested, this, &GameController::mainMenuRequested);
    connect(view, &GameView::nextRound, this, &GameController::onNextRound);

    if (view->red_timer && view->blue_timer) {
        connect(view->red_timer, &VisualTimer::timedUp, this, &GameController::onTimedUp);
        connect(view->blue_timer, &VisualTimer::timedUp, this, &GameController::onTimedUp);
    }

    if (m_mage_controller) {
        connect(m_mage_controller, &MageController::switchPlayer, this, &GameController::onSwitchPlayer);
        connect(m_mage_controller, &MageController::updateCards, this, &GameController::onUpdateCards);
        connect(m_mage_controller, &MageController::cardAppend, this, &GameController::onCardAppend);
    }

    if (m_elemental_controller) {
        connect(m_elemental_controller, &ElementalController::switchPlayer, this, &GameController::onSwitchPlayer);
        connect(m_elemental_controller, &ElementalController::updateCards, this, &GameController::onUpdateCards);
        connect(m_elemental_controller, &ElementalController::addExplosion, this, &GameController::onAddExplosion);
        connect(m_elemental_controller, &ElementalController::getCurrPlayer, this, &GameController::onGettingCurrPlayer);
        connect(m_elemental_controller, &ElementalController::cardAppend, this, &GameController::onCardAppend);
        //connect(m_elemental_controller, &ElementalController::cardAppend, this, &GameController::onCardAppend);
    }
}

void GameController::_initVisuals() {
    bool is_training{ dynamic_cast<base::TrainingMode*>(model.get()) != nullptr };
    bool is_small_timed = [this]() {
        if (auto* mode = dynamic_cast<base::TimedMode*>(model.get()); mode) {
            return mode->getGameSizeType() == base::GameSizeType::SMALL;
        }
        return false;
        }();

    qreal cardStartPos;
    if (is_training || is_small_timed) {
        cardStartPos = 360;
    }
    else {
        cardStartPos = 240;
    }

    if (dynamic_cast<base::TournamentMode*>(model.get())) {
        view->drawArena(model->getGameSizeType());
    }

    if (auto* mode = dynamic_cast<base::TimedMode*>(model.get())) {
        if (mode->getArenaService().has_value()) {
            view->drawArena(mode->getGameSizeType());
        }

        view->drawTimers(mode->getTime());
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