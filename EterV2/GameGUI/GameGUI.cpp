#include "GameGUI.h"

#include <ranges>

GameGUI::GameGUI(QWidget* parent)
    : QMainWindow(parent),
    scene{ new QGraphicsScene(this) },
    view{ new QGraphicsView(scene, this) },
    gamemode{ base::GameModeFactory::get("200", { "titi", "gigi" })}
    /*player_one{gamemode->getPlayerRed()},
    player_two{gamemode->getPlayerBlue()},
    game_board{gamemode->getBoard()}*/
{

    setWindowTitle("Eter");
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    scene->setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    view->setGeometry(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    view->setMinimumSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    drawPlayerCards(gamemode->getPlayerRed(), {100, 100});
    drawPlayerCards(gamemode->getPlayerBlue(), { 100, 700 });
    drawAvailablePositions();
}

GameGUI::~GameGUI() {}

void GameGUI::createCardAt(color::ColorType color, base::CombatCardType type, QPointF pos) {
    QString imagePath = QString(CARD_PATH)
        .arg(color == color::ColorType::RED ? "red" : "blue")
        .arg(combatCardToChar(type));

    Card* card = new Card(color, type, imagePath);
    connect(card, &Card::cardAppend, this, &GameGUI::onCardAppend);
    card->setPos(pos);
    scene->addItem(card);
}

void GameGUI::drawSquareAt(QPoint pos) {
    QGraphicsRectItem* cell = new QGraphicsRectItem(pos.x() - CARD_SIZE / 2,
        pos.y() - CARD_SIZE / 2,
        CARD_SIZE, CARD_SIZE
    );
    cell->setPen(QPen(Qt::green));    
    cell->setBrush(Qt::transparent);  
    scene->addItem(cell);
}

void GameGUI::drawAvailablePositions() {
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
    fucntie de conversie din base::Coord in QPointF si invers
    clasa Board
    clasa GamemodeController -> care se va ocupa de joc
    clasa MainWindow
*/

void GameGUI::drawPlayerCards(const base::Player& player, QPointF start_point) {
    QPointF curr_point{ start_point };

    const auto& cards = player.getCards();

    for (const auto& [type, _] : cards) {
        createCardAt(player.getColor(), type, curr_point);

        curr_point += QPointF{CARD_SIZE, 0};
    }

}

void GameGUI::onCardAppend(color::ColorType color, base::CombatCardType type, QPoint coord) {
    base::InputHandler input;
    input.event_type = base::EventType::PlaceCombatCard;
    input.card_type = type;
    input.coord = base::Coord{ coord.x(), coord.y() };

    gamemode->placeCombatCard(input);
    gamemode->switchPlayer();
    drawAvailablePositions();
}

Card::Card(color::ColorType color, base::CombatCardType type, const QString& imagePath, QGraphicsItem* parent)
    : QGraphicsItem(parent), cardImage(imagePath),
    color{ color },
    type{type} {

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