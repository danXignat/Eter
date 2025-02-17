#include "GameView.h"

#include "settings.h"

GameView::GameView(const QString& name_red, const QString& name_blue, QWidget* parent) :
    QGraphicsView{ parent },
    scene{ new QGraphicsScene(this) },
    explosion{ nullptr },
    vortex{ nullptr },
    mage_card_red{ nullptr },
    mage_card_blue{ nullptr } {

    setScene(scene);
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    scene->setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    QPixmap backgroundPixmap("../pictures/backround_v2.png");
    QPixmap scaledPixmap = backgroundPixmap.scaled(WINDOW_WIDTH, WINDOW_HEIGHT,
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation);
    scene->setBackgroundBrush(QBrush(scaledPixmap));

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSceneRect(scene->sceneRect());
    setRenderHint(QPainter::Antialiasing);
    setInteractive(true);
    _initLabels(name_red, name_blue);

    target_zone = new TargetZone();
    scene->addItem(target_zone);
    target_zone->hide();
}

Card* GameView::_createCardAt(color::ColorType color, base::CombatCardType type, QPointF pos, uint16_t id) {
    Card* card = new Card(color, type, pos, id);
    connect(card, &Card::cardAppend,        this, &GameView::cardAppend);
    connect(card, &Card::selectIllusion,    this, &GameView::selectIllusion);
    connect(card, &Card::cardStartMoving,   this, &GameView::cardStartMoving);
    connect(card, &Card::cardStoppedMoving, this, &GameView::cardStoppedMoving);
    connect(card, &Card::clickedOnRemove,   this, &GameView::clickedOnRemove);

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

    for (const auto& [pos, cell] : m_available_cells.asKeyValueRange()) {
        if (availableSpaces.contains(gui::utils::qPointFToCoord(pos)) == false) {
            m_available_cells.remove(pos);
            scene->removeItem(cell);
        }
    }
}

void GameView::drawMages(base::MageTypeAbility mage_red, const QString& desc_red, base::MageTypeAbility mage_blue, const QString& desc_blue) {
    mage_card_red = new MageCard(mage_red, desc_red, color::ColorType::RED);
    mage_card_blue = new MageCard(mage_blue, desc_blue, color::ColorType::BLUE);

    mage_card_red->setTargetZone(target_zone);
    mage_card_blue->setTargetZone(target_zone);

    connect(mage_card_red, &MageCard::applyMageCard, this, &GameView::applyMageCard);
    connect(mage_card_blue, &MageCard::applyMageCard, this, &GameView::applyMageCard);

    scene->addItem(mage_card_red);
    scene->addItem(mage_card_blue);
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

void GameView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);
}

void GameView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void GameView::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);
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

        if (mage_card_red && mage_card_red->isUsed() == false) {
            mage_card_red->setVisible(visible);
        }
        if (mage_card_blue && mage_card_blue->isUsed() == false) {
            mage_card_blue->setVisible(!visible);
        }
        };

    if (color == color::ColorType::RED) {
        change_visibility(true);
    }
    else {
        change_visibility(false);
    }
}

void GameView::setExplosionActive() {
    vortex = new Vortex();
    scene->addItem(vortex);

    explosion->setActive();
    explosion->setTargetZone(target_zone);
}

void GameView::eraseExplosion() {
    explosion->deleteLater();
    vortex->deleteLater();
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

QHash<QPointF, QList<Card*>>& GameView::getAllCardsByPos() {
    return board_cards;
}

TargetZone* GameView::getTargetZone() {
    return target_zone;
}

void GameView::highlightCards(const std::unordered_set<uint16_t>& valid_choices, CardState state1, CardState state2) {
    for (Card* card : all_cards) {
        if (card->isPlaced() == false) {
            continue;
        }

        if (valid_choices.contains(card->getID())) {
            card->setState(state1);
        }
        else {
            card->setState(state2);
        }
    }
}

void GameView::resetCardHighlight() {
    for (Card* card : all_cards) {
        card->setState(CardState::DEFAULT);
    }
}

void GameView::showAvailableSpaces(std::unordered_set<base::Coord, base::utils::CoordFunctor> coords) {
    for (BoardCell* cell : m_available_cells) {
        QPointF pos{ cell->sceneBoundingRect().center() };

        if (coords.contains(gui::utils::qPointFToCoord(pos))) {
            cell->show();
        }
        else {
            cell->hide();
        }
    }
}

void GameView::showAllAvailableSpaces() {
    for (BoardCell* cell : m_available_cells) {
        cell->show();
    }
}

void GameView::hideAllAvailableSpaces() {
    for (BoardCell* cell : m_available_cells) {
        cell->hide();
    }
}

void GameView::connectCards() {
    for (Card* card : all_cards) {
        card->disconnect();

        connect(card, &Card::cardAppend, this, &GameView::cardAppend);
        connect(card, &Card::selectIllusion, this, &GameView::selectIllusion);
        connect(card, &Card::cardStartMoving, this, &GameView::cardStartMoving);
        connect(card, &Card::cardStoppedMoving, this, &GameView::cardStoppedMoving);
        connect(card, &Card::clickedOnRemove, this, &GameView::clickedOnRemove);
    }
}

void GameView::setDeckCardsMovability(color::ColorType color, bool move) {
    for (Card* card : all_cards) {
        if (card->getColor() == color && card->isPlaced() == false) {
            card->setFlag(QGraphicsItem::ItemIsMovable, move);
            card->setFlag(QGraphicsItem::ItemIsSelectable, move);
        }
    }
}

void GameView::makeCardsMovable(const std::unordered_set<uint16_t>& ids, bool move) {
    for (uint16_t id : ids) {
        all_cards[id]->setFlag(QGraphicsItem::ItemIsMovable, move);
        all_cards[id]->setFlag(QGraphicsItem::ItemIsSelectable, move);
    }
}

void GameView::setStackMoveEvent(const std::unordered_set<uint16_t>& ids, bool in_event) {
    for (uint16_t id : ids) {
        all_cards[id]->setStackMoveEvent(in_event);
    }
}

void GameView::moveCards(const std::unordered_set<uint16_t>& ids, const base::Coord& to) {
    for (uint16_t id : ids) {
        all_cards[id]->setPos(gui::utils::coordToQPointF(to));
    }
}

void GameView::drawAvailableSpaces(const std::unordered_set<base::Coord, base::utils::CoordFunctor>& coords) {
    for (const auto& coord : coords) {
        QPointF pos{ gui::utils::coordToQPointF(coord) };

        if (m_available_cells.contains(pos) == false) {
            _drawSquareAt(pos);
        }
        else {
            m_available_cells[pos]->show();
        }
    }
}

void GameView::showCards(std::function<bool(Card*)> condition) {
    for (Card* card : all_cards) {
        if (condition(card)) {
            card->show();
        }
    }
}

void GameView::hideCards(std::function<bool(Card*)> condition) {
    for (Card* card : all_cards) {
        if (condition(card)) {
            card->hide();
        }
    }
}

void GameView::drawArena(base::GameSizeType game_size) {
    arena = new Arena(game_size);
    scene->addItem(arena);
}