#include "game.h"

#include "settings.h"

GameView::GameView(const QString& name_red, const QString& name_blue, QWidget* parent) :
	QGraphicsView{ parent },
	scene{ new QGraphicsScene(this) } {
    
	setScene(scene);
    scene->setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    this->setGeometry(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    this->setMinimumSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    this->setRenderHint(QPainter::Antialiasing);
    this->setInteractive(true);

    _initLabels(name_red, name_blue);
    //_initWebView();
}

Card* GameView::_createCardAt(color::ColorType color, base::CombatCardType type, QPointF pos) {
    QString imagePath = QString(CARD_PATH)
        .arg(color == color::ColorType::RED ? "red" : "blue")
        .arg(combatCardToChar(type));

    Card* card = new Card(color, type, imagePath);
    connect(card, &Card::cardAppend, this, &GameView::cardAppend);
    card->setPos(pos);
    scene->addItem(card);

    return card;
}

void GameView::drawPlayerCards(const base::Player& player, QPointF start_point) {
    QPointF curr_point{ start_point };

    const auto& cards = player.getCards();

    for (const auto& [type, _] : cards) {
        Card* card{ _createCardAt(player.getColor(), type, curr_point) };

        if (player.getColor() == color::ColorType::RED) {
            red_deck.push_back(card);
        }
        else {
            blue_deck.push_back(card);
        }

        curr_point += QPointF{ CARD_SIZE, 0 };
    }

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
    won_label->move(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    won_label->resize(200, 30);
    won_label->setVisible(false);
}

//void GameView::showWin(color::ColorType color) {
//    QString playerName = (color == color::ColorType::RED) ?
//        red_name_label->text() : blue_name_label->text();
//
//    // Escape playerName to prevent injection attacks
//    playerName = playerName.toHtmlEscaped();
//
//    QString simpleHtml = QString(R"(
//    <!DOCTYPE html>
//    <html>
//    <head>
//        <title>Test</title>
//    </head>
//    <body>
//        <h1>Hello, World!</h1>
//    </body>
//    </html>
//    )");
//
//    webView->setHtml(simpleHtml);
//    webView->setVisible(true);
//}
//
//void GameView::_initWebView() {
//    webView = new QWebEngineView(this);
//
//    connect(webView, &QWebEngineView::loadFinished, this, [this](bool success) {
//        if (!success) {
//            qWarning("Failed to load the web view.");
//        }
//        });
//
//    // Dynamically size and position
//    const int width = 400;
//    const int height = 200;
//    webView->resize(width, height);
//    webView->move((WINDOW_WIDTH - width) / 2, (WINDOW_HEIGHT - height) / 2);
//
//    webView->setVisible(false);
//}

void GameView::_drawSquareAt(QPointF pos) {
    int squareX = pos.x() - CARD_SIZE / 2;
    int squareY = pos.y() - CARD_SIZE / 2;
    QGraphicsRectItem* cell = new QGraphicsRectItem(squareX, squareY,
        CARD_SIZE, CARD_SIZE
    );

    QPen pen;
    pen.setColor(QColor(57, 255, 20));
    cell->setPen(pen);
    cell->setBrush(Qt::transparent);
    m_available_cells.emplace(QPointF{ pos }, cell);
    scene->addItem(cell);
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

void GameView::setDeckVisible(color::ColorType color, bool visible) {
    if (color == color::ColorType::RED) {
        red_name_label->setVisible(visible);
        for (auto card : red_deck) {
            if (card->isPlaced() == false) {
                card->setVisible(visible);
            }
        }
    }
    else {
        blue_name_label->setVisible(visible);
        for (auto card : blue_deck) {
            if (card->isPlaced() == false) {
                card->setVisible(visible);
            }
        }
    }
}

void GameView::showWin(color::ColorType color) {
    QString player_name{ (color == color::ColorType::RED) ? "RED" : "BLUE"};

    QString text{ player_name + " won" };

    won_label->setText(text);
    won_label->setVisible(true);
}

GameController::GameController(
    QWidget* parent, 
    const std::string& mode, 
    const QString& name_red, const QString& name_blue
) : QObject{ parent },
    model{ base::GameModeFactory::get(mode, {name_red.toStdString(), name_blue.toStdString()}) },
    view{ new GameView(name_red, name_blue, parent) } {

    qreal cardStartPos;
    if (mode == "100") {
        cardStartPos = 360;
    }
    else {
        cardStartPos = 240;
    }

    connect(view, &GameView::cardAppend, this, &GameController::onCardAppend);
    view->drawPlayerCards(model->getPlayerRed(), { cardStartPos, 700 });
    view->drawPlayerCards(model->getPlayerBlue(), { cardStartPos, 700 });
    view->drawAvailablePositions(model->getBoard());

    using enum color::ColorType;
    color::ColorType color{ model->getCurrPlayer().getColor() };
    color::ColorType other_color{ (color == RED) ? BLUE : RED };

    view->setDeckVisible(color, true);
    view->setDeckVisible(other_color, false);
}

void GameController::onCardAppend(Card* card) {
    base::InputHandler input;
    input.event_type = base::EventType::PlaceCombatCard;
    input.card_type = card->getType();
    input.coord = gui::utils::qPointFToCoord(card->pos());

    if (!model->placeCombatCard(input)) {
        card->moveCardBack();
    }
    else {
        card->setPlaced();
        model->switchPlayer();
        view->drawAvailablePositions(model->getBoard());

        using enum color::ColorType;
        color::ColorType color{ model->getCurrPlayer().getColor() };
        color::ColorType other_color{ (color == RED) ? BLUE : RED };

        view->setDeckVisible(color, true);
        view->setDeckVisible(other_color, false);

        if (auto win_color = model->getWinningColor(); win_color.has_value()) {
            view->showWin(win_color.value());
        }
    }
}