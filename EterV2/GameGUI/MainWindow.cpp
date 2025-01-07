#include "MainWindow.h"

#include <ranges>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    scene{ new QGraphicsScene(this) },
    view{ new QGraphicsView(scene, this) },
    gamemode{ nullptr }
    /*player_one{gamemode->getPlayerRed()},
    player_two{gamemode->getPlayerBlue()},
    game_board{gamemode->getBoard()}*/
{

    setWindowTitle("Eter");
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    scene->setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    view->setGeometry(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    view->setMinimumSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    QPushButton* trainingButton = new QPushButton("Training Mode", this);
    QPushButton* mageDuelButton = new QPushButton("Mage Duel Mode", this);
    QPushButton* elementalBattleButton = new QPushButton("Elemental Battle Mode", this);

    int BOTTON_WIDTH = 200;
    int BOTTON_HEIGHT = 60;

    trainingButton->setGeometry(WINDOW_WIDTH / 2 - BOTTON_WIDTH/2, WINDOW_HEIGHT / 2 - 90, BOTTON_WIDTH, BOTTON_HEIGHT);
    mageDuelButton->setGeometry(WINDOW_WIDTH / 2 - BOTTON_WIDTH / 2, WINDOW_HEIGHT / 2 - 20, BOTTON_WIDTH, BOTTON_HEIGHT);
    elementalBattleButton->setGeometry(WINDOW_WIDTH / 2 - BOTTON_WIDTH / 2, WINDOW_HEIGHT / 2 + 50, BOTTON_WIDTH, BOTTON_HEIGHT);
    connect(trainingButton, &QPushButton::clicked, this, &MainWindow::onTrainingModeSelected);
    connect(mageDuelButton, &QPushButton::clicked, this, &MainWindow::onMageDuelModeSelected);
    connect(elementalBattleButton, &QPushButton::clicked, this, &MainWindow::onElementalBattleModeSelected);

}

MainWindow::~MainWindow() {}

void MainWindow::createCardAt(color::ColorType color, base::CombatCardType type, QPointF pos) {
    QString imagePath = QString(CARD_PATH)
        .arg(color == color::ColorType::RED ? "red" : "blue")
        .arg(combatCardToChar(type));

    Card* card = new Card(color, type, imagePath);
    connect(card, &Card::cardAppend, this, &MainWindow::onCardAppend);
    card->setPos(pos);
    scene->addItem(card);
}

void MainWindow::drawSquareAt(QPoint pos) {

    int squareX = pos.x() - CARD_SIZE / 2;
    int squareY = pos.y() - CARD_SIZE / 2;
    QGraphicsRectItem* cell = new QGraphicsRectItem(squareX, squareY,
        CARD_SIZE, CARD_SIZE
    );
    cell->setPen(QPen(Qt::green));
    cell->setBrush(Qt::transparent);
    scene->addItem(cell);
}

void MainWindow::drawAvailablePositions() {
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

void MainWindow::drawPlayerCards(const base::Player& player, QPointF start_point) {
    QPointF curr_point{ start_point };

    const auto& cards = player.getCards();

    for (const auto& [type, _] : cards) {
        createCardAt(player.getColor(), type, curr_point);

        curr_point += QPointF{ CARD_SIZE, 0 };
    }

}

void MainWindow::onCardAppend(color::ColorType color, base::CombatCardType type, QPoint coord) {
    base::InputHandler input;
    input.event_type = base::EventType::PlaceCombatCard;
    input.card_type = type;
    input.coord = base::Coord{ coord.x(), coord.y() };

    gamemode->placeCombatCard(input);
    gamemode->switchPlayer();
    drawAvailablePositions();
}
void MainWindow::onTrainingModeSelected() {
    initializeGameMode("100"); 
}

void MainWindow::onMageDuelModeSelected() {
    initializeGameMode("200"); 
}

void MainWindow::onElementalBattleModeSelected() {
    initializeGameMode("300"); 
}

void MainWindow::initializeGameMode(const std::string& modeId) {
    gamemode = base::GameModeFactory::get(modeId, { "titi", "gigi" });

    for (QWidget* widget : findChildren<QWidget*>()) {
        if (QPushButton* button = qobject_cast<QPushButton*>(widget)) {
            button->hide();
            delete button;
        }
    }

    qreal cardStartPos;

    if (modeId == "100") {
        cardStartPos = 360;
    }
    else
        cardStartPos = 240;

    drawPlayerCards(gamemode->getPlayerRed(), { cardStartPos, 100 });
    drawPlayerCards(gamemode->getPlayerBlue(), { cardStartPos, 700 });

    drawAvailablePositions();

}

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

QPointF coordToQPointF(const base::Coord& coord) {
       return QPointF(static_cast<qreal>(coord.first), static_cast<qreal>(coord.second));
   }

base::Coord qPointFToCoord(const QPointF& point) {
    return base::Coord{ static_cast<int16_t>(point.x()), static_cast<int>(point.y()) };
}
/*


GameModeCotroler 

place card -> aplica mutarea in backend 
              si dupa o vezi in view


mainwindow sa se ocupe de scene
si gamecontroler
*/