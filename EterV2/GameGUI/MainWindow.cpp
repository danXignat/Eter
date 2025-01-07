#include "MainWindow.h"

#include <ranges>

Card::Card(color::ColorType color, base::CombatCardType type, const QString& imagePath, QGraphicsItem* parent)
    : QGraphicsItem(parent), cardImage(imagePath),
    color{ color },
    type{ type } {

    if (cardImage.isNull()) {
        qWarning() << "Failed to load card image:" << imagePath;
    }

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

QRectF Card::boundingRect() const {
    return QRectF(-CARD_SIZE / 2, -CARD_SIZE / 2, CARD_SIZE, CARD_SIZE);
}

void Card::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPixmap scaledImage = cardImage.scaled(CARD_SIZE, CARD_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter->drawPixmap(-CARD_SIZE / 2, -CARD_SIZE / 2, scaledImage);
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    lastMousePosition = event->scenePos();

    qDebug() << "pressed" << "\n";

    QGraphicsItem::mousePressEvent(event);
}

void Card::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QPointF delta = event->scenePos() - lastMousePosition;

    setPos(pos() + delta);

    lastMousePosition = event->scenePos();
    QGraphicsItem::mouseMoveEvent(event);
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QPointF cardCenter = sceneBoundingRect().center();  // Get card center in scene coordinates
    QList<QGraphicsItem*> itemsUnderCard = scene()->items(cardCenter); // Get items at center

    for (QGraphicsItem* item : itemsUnderCard) {
        auto* cell = dynamic_cast<QGraphicsRectItem*>(item);
        if (cell) {
            QPointF target_coord{ cell->sceneBoundingRect().center() };

            setPos(target_coord);
            emit cardAppend(color, type, target_coord.toPoint());

            break;
        }
    }

    qDebug() << "Card snapped to:" << pos();
    QGraphicsItem::mouseReleaseEvent(event);
}

//todo  fa un headerr utils unde pui functiile astea de conversie
QPointF coordToQPointF(const base::Coord& coord) {
    return QPointF(static_cast<qreal>(coord.first), static_cast<qreal>(coord.second));
}

base::Coord qPointFToCoord(const QPointF& point) {
    return base::Coord{ static_cast<int16_t>(point.x()), static_cast<int>(point.y()) };
}

RequestNameScene::RequestNameScene(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("Enter your name");
    layout->addWidget(nameInput);

    nextButton = new QPushButton("Next", this);
    layout->addWidget(nextButton);

    connect(nextButton, &QPushButton::clicked, this, &RequestNameScene::onNextClicked);
}

void RequestNameScene::onNextClicked() {
    emit nameEntered(nameInput->text());
}

SelectModeScene::SelectModeScene(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    easyButton = new QPushButton("Training", this);
    hardButton = new QPushButton("Mage", this);
    layout->addWidget(easyButton);
    layout->addWidget(hardButton);

    connect(easyButton, &QPushButton::clicked, this, [=]() { emit modeSelected("100"); });
    connect(hardButton, &QPushButton::clicked, this, [=]() { emit modeSelected("200"); });
}

GameScene::GameScene(QWidget* parent) : QWidget(parent),
    scene{ new QGraphicsScene(this) },
    view{ new QGraphicsView(scene, this) },
    gamemode{ nullptr }
    /*player_one{gamemode->getPlayerRed()},
    player_two{gamemode->getPlayerBlue()},
    game_board{gamemode->getBoard()}*/
{
    scene->setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    view->setGeometry(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    view->setMinimumSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    view->setRenderHint(QPainter::Antialiasing);
    view->setInteractive(true);
}

void GameScene::startGame(const std::string& mode, const std::string& playerName) {
    gamemode = base::GameModeFactory::get(mode, { playerName, playerName });

    qreal cardStartPos;

    if (mode == "100") {
        cardStartPos = 360;
    }
    else {
        cardStartPos = 240;
    }

    drawPlayerCards(gamemode->getPlayerRed(), { cardStartPos, 100 });
    drawPlayerCards(gamemode->getPlayerBlue(), { cardStartPos, 700 });

    drawAvailablePositions();
}

void GameScene::createCardAt(color::ColorType color, base::CombatCardType type, QPointF pos) {
    QString imagePath = QString(CARD_PATH)
        .arg(color == color::ColorType::RED ? "red" : "blue")
        .arg(combatCardToChar(type));

    Card* card = new Card(color, type, imagePath);
    connect(card, &Card::cardAppend, this, &GameScene::onCardAppend);
    card->setPos(pos);
    scene->addItem(card);
}

void GameScene::drawSquareAt(QPoint pos) {

    int squareX = pos.x() - CARD_SIZE / 2;
    int squareY = pos.y() - CARD_SIZE / 2;
    QGraphicsRectItem* cell = new QGraphicsRectItem(squareX, squareY,
        CARD_SIZE, CARD_SIZE
    );
    cell->setPen(QPen(Qt::green));
    cell->setBrush(Qt::transparent);
    scene->addItem(cell);
}

void GameScene::drawAvailablePositions() {
    const auto& availableSpaces = gamemode->getBoard().availableSpaces();

    for (const auto& [x, y] : availableSpaces) {
        drawSquareAt({ x, y });
    }

    for (QGraphicsItem* item : scene->items()) {
        if (QGraphicsRectItem* rectItem = dynamic_cast<QGraphicsRectItem*>(item)) {
            base::Coord pos{
                rectItem->boundingRect().center().toPoint().x(),
                rectItem->boundingRect().center().toPoint().y()
            };

            if (availableSpaces.contains(pos) == false) {
                scene->removeItem(rectItem);
                delete rectItem;
            }
        }
    }
}

/*
    TODO:
    eficientizare
    functie de conversie din base::Coord in QPointF si invers
    clasa Board
    clasa GamemodeController -> care se va ocupa de joc
    clasa MainWindow
*/

void GameScene::drawPlayerCards(const base::Player& player, QPointF start_point) {
    QPointF curr_point{ start_point };

    const auto& cards = player.getCards();

    for (const auto& [type, _] : cards) {
        createCardAt(player.getColor(), type, curr_point);

        curr_point += QPointF{ CARD_SIZE, 0 };
    }

}

void GameScene::onCardAppend(color::ColorType color, base::CombatCardType type, QPoint coord) {
    base::InputHandler input;
    input.event_type = base::EventType::PlaceCombatCard;
    input.card_type = type;
    input.coord = base::Coord{ coord.x(), coord.y() };

    gamemode->placeCombatCard(input);
    gamemode->switchPlayer();
    drawAvailablePositions();
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {

    setWindowTitle("Eter");
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    stackedWidget = new QStackedWidget(this);

    // Create scenes
    requestNameScene = new RequestNameScene();
    selectModeScene = new SelectModeScene();
    gameScene = new GameScene();

    // Add scenes to the QStackedWidget
    stackedWidget->addWidget(requestNameScene); // Index 0
    stackedWidget->addWidget(selectModeScene);  // Index 1
    stackedWidget->addWidget(gameScene);        // Index 2

    setCentralWidget(stackedWidget);

    // Connect signals and slots for transitions
    connect(requestNameScene, &RequestNameScene::nameEntered, this, &MainWindow::onNameEntered);
    connect(selectModeScene, &SelectModeScene::modeSelected, this, &MainWindow::onModeSelected);
}

void MainWindow::onNameEntered(const QString& playerName) {
    // Handle player name, then switch to the mode selection scene
    playerNameGlobal = playerName; // Store globally if needed
    stackedWidget->setCurrentIndex(1); // Switch to SelectModeScene
}

void MainWindow::onModeSelected(const std::string& mode) {
    // Handle mode selection, then switch to the game scene
    selectedMode = mode; // Store globally if needed
    stackedWidget->setCurrentIndex(2); // Switch to GameScene
    gameScene->startGame(selectedMode, playerNameGlobal.toStdString()); // Initialize the game
}