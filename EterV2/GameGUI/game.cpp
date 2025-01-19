#include "game.h"

#include "settings.h"

GameView::GameView(const QString& name_red, const QString& name_blue, QWidget* parent) :
    QGraphicsView{ parent },
    scene{ new QGraphicsScene(this) },
    explosion{ nullptr },
    explosion_target_zone{ nullptr },
    vortex{ nullptr } {
    setScene(scene);

    // Set fixed size first
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Set scene rect to match window size
    scene->setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Scale background to match the WINDOW size, not viewport size
    QPixmap backgroundPixmap("../pictures/arena.png");
    QPixmap scaledPixmap = backgroundPixmap.scaled(WINDOW_WIDTH, WINDOW_HEIGHT,
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation);
    scene->setBackgroundBrush(QBrush(scaledPixmap));

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSceneRect(scene->sceneRect());
    setRenderHint(QPainter::Antialiasing);
    setInteractive(true);
    _initLabels(name_red, name_blue);  // Removed asterisks that looked like a typo
}

Card* GameView::_createCardAt(color::ColorType color, base::CombatCardType type, QPointF pos, uint16_t id) {
    QString image_path = QString(CARD_PATH)
        .arg(color == color::ColorType::RED ? "red" : "blue")
        .arg(combatCardToChar(type));
    QString backPath = QString(BACK_PATH)
        .arg(color == color::ColorType::RED ? "red" : "blue")
        .arg("back");
    Card* card = new Card(color, type, image_path, backPath, pos, id);
    connect(card, &Card::cardAppend, this, &GameView::cardAppend);
    scene->addItem(card);

    return card;
}

void GameView::_initLabels(const QString& name_red, const QString& name_blue) {
    red_name_label = new QLabel(this);
    red_name_label->setText(name_red + "'s cards:");
    red_name_label->setStyleSheet("font-size: 18px; font-weight: bold;");
    red_name_label->move(70, WINDOW_HEIGHT - 150);
    red_name_label->resize(200, 30);

    blue_name_label = new QLabel(this);
    blue_name_label->setText(name_blue + "'s cards:");
    blue_name_label->setStyleSheet("font-size: 18px; font-weight: bold;");
    blue_name_label->move(70, WINDOW_HEIGHT - 150);
    blue_name_label->resize(200, 30);

    won_label = new QLabel(this);
    won_label->setStyleSheet("font-size: 28px; font-weight: bold;");
    won_label->move(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    won_label->resize(200, 30);
    won_label->setVisible(false);
}

void GameView::_drawSquareAt(QPointF pos) {
    int square_x = pos.x() - CARD_SIZE / 2;
    int square_y = pos.y() - CARD_SIZE / 2;

    BoardCell* cell{ new BoardCell{ {double(square_x), double(square_y)} } };
    m_available_cells.emplace(QPointF{ pos }, cell);
    scene->addItem(cell);
}

void GameView::cardAppendBoard(Card* card) {
    QPointF card_pos{ card->pos() };
    qDebug() << card->pos() << "\n";

    if (board_cards.contains(card_pos)) {
        board_cards[card_pos].push_back(card);
    }
    else {
        QList<Card*> stack;
        stack.push_back(card);
        board_cards.emplace(card->pos(), stack);
    }

    emit cardAppend(card);
}

void GameView::drawPlayerCards(const base::Player& player, QPointF start_point) {
    QPointF curr_point{ start_point };

    const auto& cards = player.getCards();

    for (const auto& [type, card] : cards) {
        Card* card_view{ _createCardAt(player.getColor(), type, curr_point, card.getID()) };

        all_cards.emplace(card.getID(), card_view);
        if (player.getColor() == color::ColorType::RED) {
            red_deck.push_back(card_view);
        }
        else {
            blue_deck.push_back(card_view);
        }

        curr_point += QPointF{ CARD_SIZE, 0 };
    }

}

void GameView::drawExplosion(const base::ExplosionService& service, uint16_t board_size, QPointF start_point) {
    explosion = new Explosion(service.getEffectCoords(), board_size);

    connect(explosion, &Explosion::apply, this, &GameView::explosionApply);
    connect(explosion, &Explosion::discard, this, &GameView::explosionDiscard);

    scene->addItem(explosion);
    explosion->setPos(start_point);
}

void GameView::drawAvailablePositions(const base::Board& board) {
    const auto& availableSpaces = board.availableSpaces();

    for (const auto& coord : availableSpaces) {
        QPointF pos{ gui::utils::coordToQPointF(coord) };
        if (m_available_cells.contains(pos) == false) {
            _drawSquareAt(pos);
        }
    }

    for (const auto&[pos, cell] : m_available_cells.asKeyValueRange()) {
        if (availableSpaces.contains(gui::utils::qPointFToCoord(pos)) == false) {
            m_available_cells.remove(pos);
            scene->removeItem(cell);
        }
    }
}

void GameView::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Left) {
        explosion->setRotation(explosion->rotation() - 90);
        emit explosionRotateLeft();
    }
    else if (event->key() == Qt::Key_Right) {
        explosion->setRotation(explosion->rotation() + 90);
        emit explosionRotateRight();
    }
    else {
        QGraphicsView::keyPressEvent(event);
    }
}

void GameView::switchToPlayer(const base::Player& player) {
    color::ColorType color = player.getColor();

    auto change_visibility = [&](bool visible) {
        red_name_label->setVisible(visible);
        for (auto card : red_deck) {
            if (card->isPlaced() == false && card->isUsed() == false) {
                card->setVisible(visible);
            }
        }

        blue_name_label->setVisible(!visible);
        for (auto card : blue_deck) {
            if (card->isPlaced() == false && card->isUsed() == false) {
                card->setVisible(!visible);
            }
        }
        };

    if (color == color::ColorType::RED) {
        change_visibility(true);
    }
    else {
        change_visibility(false);
    }
}

void GameView::setExplosionActive(const QPointF& p1, const QPointF& p2) {
    explosion_target_zone = new TargetZone(p1 - QPointF{CARD_SIZE, CARD_SIZE}, p2 + QPointF{CARD_SIZE, CARD_SIZE});
    scene->addItem(explosion_target_zone);
    
    vortex = new Vortex() ;
    scene->addItem(vortex);

    explosion->setActive();
    explosion->setTargetZone(explosion_target_zone);
}

void GameView::eraseExplosion() {
    explosion->hide();
    explosion_target_zone->hide();
    vortex->hide();
}

void GameView::drawHole(const QPointF& pos) {
    Hole* hole{ new Hole(pos) };
    scene->addItem(hole);
}

void GameView::showWin(color::ColorType color) {
    QString imagePath = (color == color::ColorType::RED)
        ? "../pictures/win/red_victory.png"
        : "../pictures/win/blue_victory.png";

    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image:" << imagePath;
        return;
    }

    QSize labelSize(1000, 500); // Set desired dimensions
    QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Set the pixmap on the QLabel
    won_label->setPixmap(scaledPixmap);
    won_label->setFixedSize(labelSize);
    won_label->move(350, 180);
    won_label->setVisible(true);
}

QHash<uint16_t, Card*>& GameView::getAllCards() {
    return all_cards;
}

GameController::GameController(
    QWidget* parent, 
    const std::string& mode, 
    const QString& name_red, const QString& name_blue
) : QObject{ parent },
    model{ base::GameModeFactory::get(mode, {name_red.toStdString(), name_blue.toStdString()}) },
    view{ new GameView(name_red, name_blue, parent) } {

    _initConections();
    _initVisuals();
}

///--------------------------------------------------------------------------SIGNALS------------------------------------------------------

void GameController::onCardAppend(Card* card) {
    base::InputHandler input;
    input.event_type = base::EventType::PlaceCombatCard;
    input.card_type = card->getType();
    input.coord = gui::utils::qPointFToCoord(card->pos());
    input.ID = card->getID();

    if (!model->placeCombatCard(input)) {
        card->moveCardBack();
    }
    else {
        bool change_player = true;

        card->setPlaced(true);
        view->drawAvailablePositions(model->getBoard());

        if (auto win_color = model->getWinningColor(); win_color.has_value()) {
            //view->showWin(win_color.value());
        }

        if (model->getExplosionService().has_value() &&
            model->getExplosionService()->checkAvailability()
            ) {
            auto [coord1, coord2]{ model->getBoard().getBoudingRect() };

            for (const auto& [id, card] : view->getAllCards().asKeyValueRange()) {
                card->setFlag(QGraphicsItem::ItemIsMovable, false);
            }

            view->setExplosionActive(gui::utils::coordToQPointF(coord1), gui::utils::coordToQPointF(coord2));
            change_player = false;
        }

        if (change_player) {
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

    for (const auto& [id, card] : view->getAllCards().asKeyValueRange()) {
        card->setFlag(QGraphicsItem::ItemIsMovable, true);
    }
}

void GameController::onExplosionDiscard() {
    model->switchPlayer();
    model->removeExplosion();
    view->eraseExplosion();
    view->switchToPlayer(model->getCurrPlayer());

    for (const auto& [id, card] : view->getAllCards().asKeyValueRange()) {
        card->setFlag(QGraphicsItem::ItemIsMovable, true);
    }
}

void GameController::onExplosionRotateLeft() {
    model->getExplosionService()->rotateLeft();
}

void GameController::onExplosionRotateRight() {
    model->getExplosionService()->rotateRight();
}

void GameController::_updateBoardView() {
    auto& card_views = view->getAllCards();
    const auto& curr_player_model = model->getCurrPlayer();
    const auto& player_red_model = model->getPlayerRed();
    const auto& player_blue_model= model->getPlayerBlue();
    const auto& board_model = model->getBoard();
    
    bool hide_red{ (curr_player_model.getColor() == color::ColorType::RED) ? false : true };

    _updatePlayerCards(player_red_model, card_views, hide_red);
    _updatePlayerCards(player_blue_model, card_views, !hide_red);
    _updateBoardCards(board_model, card_views);
}

///--------------------------------------------------------INIT-------------------------------------------------

void GameController::_initConections() {
    connect(view, &GameView::cardAppend          , this, &GameController::onCardAppend);
    connect(view, &GameView::explosionApply      , this, &GameController::onExplosionApply);
    connect(view, &GameView::explosionDiscard    , this, &GameController::onExplosionDiscard);
    connect(view, &GameView::explosionRotateLeft , this, &GameController::onExplosionRotateLeft);
    connect(view, &GameView::explosionRotateRight, this, &GameController::onExplosionRotateRight);
}

void GameController::_initVisuals() {
    qreal cardStartPos;
    if (dynamic_cast<base::TrainingMode*>(model.get())) {
        cardStartPos = 360;
    }
    else {
        cardStartPos = 240;
    }

    view->drawPlayerCards(model->getPlayerRed(), { cardStartPos, 700 });
    view->drawPlayerCards(model->getPlayerBlue(), { cardStartPos, 700 });
    view->drawAvailablePositions(model->getBoard());

    if (model->getExplosionService().has_value()) {
        view->drawExplosion(*model->getExplosionService(), model->getBoard().size(), { 1000,555 });
    }

    view->switchToPlayer(model->getCurrPlayer());
}

///----------------------------------------------------------UPDATE-------------------------------------------------

void GameController::_updatePlayerCards(const base::Player& player, QHash<uint16_t, Card*>& card_views, bool hide) {
    for (const auto& [type, card] : player.getCards()) {
        if (card.isCombatCard() == false) {
            continue;
        }

        uint16_t ID{ card.getID() };
        card_views[ID]->moveCardBack();
        card_views[ID]->setFlag(QGraphicsItem::ItemIsMovable);

        if (hide) {
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

        if (hide) {
            card_views[ID]->hide();
            card_views[ID]->setPlaced(false);
            card_views[ID]->setUsed(true);
        }
    }
}

void GameController::_updateBoardCards(const base::Board& board, QHash<uint16_t, Card*>& card_views) {
    for (const auto& [coord, stack] : board) {
        if (stack[0].getType() == base::CombatCardType::HOLE) {
            view->drawHole(gui::utils::coordToQPointF(coord));
        }
    }
}