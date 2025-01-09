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

    playerBlueNameInput = new QLineEdit(this);
    playerBlueNameInput->setPlaceholderText("Enter Player 1 Name");
    playerBlueNameInput->setFixedSize(300, 50); 
    playerBlueNameInput->setStyleSheet("font-size: 18px;"); 
    layout->addWidget(playerBlueNameInput, 0, Qt::AlignCenter);

    playerRedNameInput = new QLineEdit(this);
    playerRedNameInput->setPlaceholderText("Enter Player 2 Name");
    playerRedNameInput->setFixedSize(300, 50);
    playerRedNameInput->setStyleSheet("font-size: 18px;"); 
    layout->addWidget(playerRedNameInput, 0, Qt::AlignCenter); 

    nextButton = new QPushButton("Next", this);
    nextButton->setFixedSize(200, 50);
    nextButton->setStyleSheet("font-size: 18px;"); 
    layout->addWidget(nextButton, 0, Qt::AlignCenter); 

    connect(nextButton, &QPushButton::clicked, this, &RequestNameScene::onNextClicked);
}


void RequestNameScene::onNextClicked() {
    emit nameEntered(playerBlueNameInput->text(), playerRedNameInput->text());
}


SelectModeScene::SelectModeScene(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    trainingButton = new QPushButton("Training", this);
    trainingButton->setFixedSize(200, 50); 
    trainingButton->setStyleSheet("font-size: 18px;"); 
    layout->addWidget(trainingButton, 0, Qt::AlignCenter);

    mageDuelButton = new QPushButton("Mage-Duel", this);
    mageDuelButton->setFixedSize(200, 50); 
    mageDuelButton->setStyleSheet("font-size: 18px;"); 
    layout->addWidget(mageDuelButton, 0, Qt::AlignCenter); 

    elementalBattleButton = new QPushButton("Elemental Battle", this);
    elementalBattleButton->setFixedSize(200, 50);
    elementalBattleButton->setStyleSheet("font-size: 18px;");
    layout->addWidget(elementalBattleButton, 0, Qt::AlignCenter);

    connect(trainingButton, &QPushButton::clicked, this, [=]() { emit modeSelected("100"); });
    connect(mageDuelButton, &QPushButton::clicked, this, [=]() { emit modeSelected("200"); });
    connect(elementalBattleButton, &QPushButton::clicked, this, [=]() { emit modeSelected("300"); });

}


GameScene::GameScene(QWidget* parent) : QWidget(parent),
    scene{ new QGraphicsScene(this) },
    view{ new QGraphicsView(scene, this) },
    gamemode{ nullptr }

    /*player_one{gamemode->getPlayerRed()},
    player_two{gamemode->getPlayerBlue()},
    game_board{gamemode->getBoard()}*/
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    scene->setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    view->setGeometry(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    view->setMinimumSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    view->setRenderHint(QPainter::Antialiasing);
    view->setInteractive(true);


    playerBlueNameLabel = new QLabel(this);
    playerBlueNameLabel->setText("Player 1's cards:");
    playerBlueNameLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    playerBlueNameLabel->move(70, WINDOW_HEIGHT - 180); 
    playerBlueNameLabel->resize(200, 30);


    playerRedNameLabel = new QLabel(this);
    playerRedNameLabel->setText("Player 2's cards:");
    playerRedNameLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    playerRedNameLabel->move(70, 30);
    playerRedNameLabel->resize(200, 30);

    view->setParent(this);
}

void GameScene::startGame(const std::string& mode, const std::string& playerBlueName, const std::string& playerRedName) {
    gamemode = base::GameModeFactory::get(mode, { playerBlueName, playerRedName });

    playerBlueNameLabel->setText(QString::fromStdString(playerBlueName) + "'s cards:");
    playerRedNameLabel->setText(QString::fromStdString(playerRedName) + "'s cards:");

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

    connect(requestNameScene, &RequestNameScene::nameEntered, this, &MainWindow::onNameEntered);
    connect(selectModeScene, &SelectModeScene::modeSelected, this, &MainWindow::onModeSelected);
}

void MainWindow::onNameEntered(const QString& playerBlueName, const QString& playerRedName) {
    playerBlueNameGlobal = playerBlueName;
    playerRedNameGlobal = playerRedName;
    stackedWidget->setCurrentIndex(1);
}


void MainWindow::onModeSelected(const std::string& mode) {
    
    selectedMode = mode; 
    stackedWidget->setCurrentIndex(2); 
    gameScene->startGame(selectedMode, playerBlueNameGlobal.toStdString(), playerRedNameGlobal.toStdString()); 
}