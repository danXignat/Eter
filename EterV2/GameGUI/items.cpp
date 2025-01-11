#include "items.h"

#include "settings.h"

Card::Card(color::ColorType color, base::CombatCardType type, const QString& imagePath, QGraphicsItem* parent)
    : QGraphicsItem(parent), cardImage(imagePath),
    color{ color },
    type{ type },
    placed{ false } {

    if (cardImage.isNull()) {
        qWarning() << "Failed to load card image:" << imagePath;
    }

    setZValue(5);
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

void Card::moveCardBack() {
    this->setPos(lastCardPosition);
}
base::CombatCardType Card::getType() const {
    return type;
}
color::ColorType Card::getColor() const {
    return color;
}
void Card::setPlaced() {
    placed = true;
}

bool Card::isPlaced() const {
    return placed;
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (placed) {
        return;
    }

    lastMousePosition = event->scenePos();
    lastCardPosition = pos();
    qDebug() << "pressed" << "\n";
    QGraphicsItem::mousePressEvent(event);
}

void Card::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (placed) {
        return;
    }

    QPointF delta = event->scenePos() - lastMousePosition;

    setPos(pos() + delta);

    lastMousePosition = event->scenePos();
    QGraphicsItem::mouseMoveEvent(event);
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (placed) {
        return;
    }

    QPointF cardCenter = sceneBoundingRect().center();  // Get card center in scene coordinates
    QList<QGraphicsItem*> itemsUnderCard = scene()->items(cardCenter); // Get items at center

    for (QGraphicsItem* item : itemsUnderCard) {
        auto* cell = dynamic_cast<QGraphicsRectItem*>(item);
        if (cell) {
            QPointF target_coord{ cell->sceneBoundingRect().center() };

            setPos(target_coord);
            setZValue(1);
            break;
        }
        auto* card = dynamic_cast<Card*>(item);
        if (card && card != this) {
            QPointF target_coord{ card->sceneBoundingRect().center() };

            setPos(target_coord);
            setZValue(card->zValue() + 1);
            break;
        }
    }

    emit cardAppend(this);

    qDebug() << "Card snapped to:" << pos();
    QGraphicsItem::mouseReleaseEvent(event);
}

Explosion::Explosion(const std::unordered_map<base::Coord, base::Effect, base::utils::CoordFunctor>& effects)  
    : baseImage { EXPLOSION_PATH }
{
    if (baseImage.isNull()) {
        qWarning() << "Failed to load card image:" << EXPLOSION_PATH;
    }
    setZValue(5);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}
QRectF Explosion::boundingRect() const {
    return QRectF(-EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, EXPLOSION_SIZE, EXPLOSION_SIZE);
}
void Explosion::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPixmap scaledImage = baseImage.scaled(EXPLOSION_SIZE, EXPLOSION_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    painter->drawPixmap(-EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, scaledImage);
}