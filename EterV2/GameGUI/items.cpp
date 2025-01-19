#include "items.h"

#include "settings.h"

Card::Card(color::ColorType color, base::CombatCardType type, const QString& imagePath,const QString& backPath, QGraphicsItem* parent)
    : QGraphicsItem(parent), cardImage(imagePath), cardBack(backPath),
    color{ color },
    type{ type },
    placed{ false } {

    if (cardImage.isNull()) {
        qWarning() << "Failed to load card image:" << imagePath;
    }

    setZValue(5);
    setFlags(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
}

QRectF Card::boundingRect() const {
    return QRectF(-CARD_SIZE / 2, -CARD_SIZE / 2, CARD_SIZE, CARD_SIZE);
}

void Card::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (faceUp) {
        QPixmap scaledImage = cardImage.scaled(CARD_SIZE, CARD_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        painter->drawPixmap(-CARD_SIZE / 2, -CARD_SIZE / 2, scaledImage);
    }
    else{
        QPixmap scaledImage = cardBack.scaled(CARD_SIZE, CARD_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        painter->drawPixmap(-CARD_SIZE / 2, -CARD_SIZE / 2, scaledImage);
    }
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
    this->setFlag(QGraphicsItem::ItemIsMovable, false);
    this->setFlag(QGraphicsItem::ItemIsSelectable, false);

    this->setAcceptHoverEvents(false);
    this->setAcceptTouchEvents(false);
    this->setAcceptDrops(false);

    this->update();
}

bool Card::isPlaced() const {
    return placed;
}
void Card::flipCard(){
    faceUp = !faceUp;
}
bool Card::isFaceUp() {
    return faceUp;
}
void Card::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePosition = event->scenePos();
        lastCardPosition = pos();
        qDebug() << "Left mouse button pressed on card";
        QGraphicsItem::mousePressEvent(event); 
    }
    else {
        flipCard();
        qDebug() << "Non-left mouse button ignored on card";
        event->ignore();
    }
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

ExplosionView::ExplosionView(const std::unordered_map<base::Coord, base::Effect, base::utils::CoordFunctor>& effects, uint16_t board_size)
    : explosionImage{ new QPixmap(QPixmap{(board_size == 3) ? EXPLOSION_3x3_PATH : EXPLOSION_4x4_PATH}.scaled(EXPLOSION_SIZE, EXPLOSION_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation))},
      handImage{ new QPixmap(QPixmap{HAND_PATH}.scaled(EFFECT_SIZE, EFFECT_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) },
      removeImage{ new QPixmap(QPixmap{REMOVE_PATH}.scaled(EFFECT_SIZE, EFFECT_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) },
      holeImage{ new QPixmap(QPixmap{HOLE_PATH}.scaled(EFFECT_SIZE, EFFECT_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) },
      m_board_size{board_size},
      m_cell_size{ static_cast<double>(EXPLOSION_SIZE) / board_size },
      active{false},
    m_zone{nullptr}
{
    setZValue(7);

    for (const auto& [coord, effect] : effects) {
        QPointF mapped{ this->mapEffectToCard(coord) };

        switch (effect)
        {
        case base::Effect::HAND:
            m_effects.emplace(mapped, handImage);
            break;

        case base::Effect::REMOVE:
            m_effects.emplace(mapped, removeImage);
            break;

        case base::Effect::HOLE:
            m_effects.emplace(mapped, holeImage);
            break;

        default:
            break;
        }
    }
}

QRectF ExplosionView::boundingRect() const {
    return QRectF(-EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, EXPLOSION_SIZE, EXPLOSION_SIZE);
}

void ExplosionView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(-EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, *explosionImage);

    for (const auto& [pos, image] : m_effects.asKeyValueRange()) {
        painter->drawPixmap(pos.x(), pos.y(), *image);
    }

    QPen pen;
    QBrush brush;
    pen.setWidth(2);

    if (active) {
        pen.setColor(QColor(0, 255, 0));
        brush.setStyle(Qt::NoBrush);
    }
    else {
        QColor semiTransparentRed(255, 0, 0, 127);
        brush.setColor(semiTransparentRed);
        brush.setStyle(Qt::SolidPattern);
        pen.setColor(QColor(255, 0, 0));
    }

    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawRect(QRect{ -EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, EXPLOSION_SIZE, EXPLOSION_SIZE });
}
QPointF ExplosionView::mapEffectToCard(const base::Coord& coord) {
    QPointF casted{ gui::utils::coordToQPointF(coord) };
    QPointF swapped{ casted.y(), casted.x() };
    QPointF translated{ swapped + ((m_board_size == 3) ? QPointF{-1, -1 } : QPointF{-double(3) / 2, -double(3) / 2}) };
    QPointF scaled{ m_cell_size * translated };

    QPointF center_offset{ -EFFECT_SIZE / 2, -EFFECT_SIZE / 2 };

    return QPointF{ scaled + center_offset };
}

void ExplosionView::setActive() {
    active = true;
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
    
    setFocus();

    this->update();
}

void ExplosionView::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Left) {
        this->setRotation(this->rotation() - 90);
    }
    else if (event->key() == Qt::Key_Right) {
        this->setRotation(this->rotation() + 90);
    }
    else {
        QGraphicsItem::keyPressEvent(event);
    }
}

void ExplosionView::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    dragOffset = pos() - mapToScene(event->pos());
    lastCardPosition = pos();

    qDebug() << "pressed" << "\n";
    QGraphicsItem::mousePressEvent(event);
}

void ExplosionView::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (m_zone->contains(this->pos())) {
        m_zone->show();
    }
    else {
        m_zone->hide();
    }

    QPointF newPos = mapToScene(event->pos()) + dragOffset;

    // Optional: Add bounds checking
    QRectF sceneRect = scene()->sceneRect();
    newPos.setX(qBound(sceneRect.left(), newPos.x(), sceneRect.right()));
    newPos.setY(qBound(sceneRect.top(), newPos.y(), sceneRect.bottom()));

    QGraphicsItem::mouseMoveEvent(event);
}

void ExplosionView::setTargetZone(TargetZone* zone) {
    m_zone = zone;
}

void ExplosionView::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseReleaseEvent(event);
}

//void ExplosionView::advance(int phase) {
//    if (!phase) {
//        return;
//    }
//
//    moveBy(5, 0);
//
//    for (QGraphicsItem* item : scene()->items(this->pos())) {
//        if (TargetZone* zone = dynamic_cast<TargetZone*>(item)) {
//            if (collidesWithItem(zone)) {
//                zone->show();
//            }
//            else {
//                zone->hide();
//            }
//        }
//    }
//}

TargetZone::TargetZone(QPointF coord1, QPointF coord2) {
    qreal x = std::min(coord1.x(), coord2.x());
    qreal y = std::min(coord1.y(), coord2.y());
    qreal width = std::abs(coord2.x() - coord1.x());
    qreal height = std::abs(coord2.y() - coord1.y());

    setZValue(6);
    setRect(x, y, width, height);

    QBrush brush;
    QColor semiTransparentRed(255, 0, 0, 127);
    brush.setColor(semiTransparentRed);
    brush.setStyle(Qt::SolidPattern);

    setBrush(Qt::NoBrush);
    setPen(Qt::NoPen);
}

void TargetZone::show() {
    if (brush().style() == Qt::SolidPattern) return;

    QBrush brush;
    QColor semiTransparentRed(255, 0, 0, 127);
    brush.setColor(semiTransparentRed);
    brush.setStyle(Qt::SolidPattern);

    setPen(QPen(Qt::DashLine));
    setBrush(brush);
    update();
}

void TargetZone::hide() {
    setBrush(Qt::NoBrush);
    setPen(Qt::NoPen);
    update();
}