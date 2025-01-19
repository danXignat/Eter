#include "items.h"

#include "settings.h"

/// -----------------------------------------------------------BOARD----------------------------------------------

BoardCell::BoardCell(const QPointF& pos)
    : QGraphicsRectItem(pos.x(), pos.y(), CARD_SIZE, CARD_SIZE) {
    QPen pen(QColor(57, 255, 20));
    pen.setWidth(5);
    setPen(pen);

    setBrush(Qt::NoBrush);
}

Card::Card(color::ColorType color, base::CombatCardType type, const QString& image_path, const QString& back_path, const QPointF& pos, uint16_t ID, QGraphicsItem* parent)
    : QGraphicsItem(parent), 
    cardImage(image_path), cardBack(back_path),
    color{ color },
    type{ type },
    placed{ false },
    used{false},
    start_pos{pos},
    m_ID{ID} {

    setPos(start_pos);
    setZValue(5);
    setFlags(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
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
    this->setPos(start_pos);
}
base::CombatCardType Card::getType() const {
    return type;
}
color::ColorType Card::getColor() const {
    return color;
}

bool Card::isUsed() const {
    return used;
}

void Card::setUsed(bool used) {
    this->used = used;
}

void Card::setPlaced(bool placed) {
    this->placed = placed;
    this->setFlag(QGraphicsItem::ItemIsMovable, !placed);
    this->setFlag(QGraphicsItem::ItemIsSelectable, !placed);

    this->setAcceptHoverEvents(!placed);
    this->setAcceptTouchEvents(!placed);
    this->setAcceptDrops(!placed);

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
        setFocus();

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
    QGraphicsItem::mouseMoveEvent(event);
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QPointF cardCenter = sceneBoundingRect().center();  // Get card center in scene coordinates
    QList<QGraphicsItem*> itemsUnderCard = scene()->items(cardCenter); // Get items at center

    for (QGraphicsItem* item : itemsUnderCard) {
        if (dynamic_cast<Explosion*>(item)) {
            continue;
        }

        auto* cell = dynamic_cast<BoardCell*>(item);
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

uint16_t Card::getID() const {
    return m_ID;
}


///----------------------------------------------------------EXPLOSION----------------------------------------------------

Hole::Hole(const QPointF& pos)
    : image{ new QPixmap(QPixmap{ HOLE_PATH }.scaled(CARD_SIZE, CARD_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation)) } {
    setPos(pos.x(), pos.y());
}

QRectF Hole::boundingRect() const {
    return QRectF(-CARD_SIZE / 2, -CARD_SIZE / 2, CARD_SIZE, CARD_SIZE);
}

void Hole::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
    painter->drawPixmap(-CARD_SIZE / 2, -CARD_SIZE / 2, *image);
    painter->restore();
}

Explosion::Explosion(const std::unordered_map<base::Coord, base::Effect, base::utils::CoordFunctor>& effects, uint16_t board_size)
    : QGraphicsItem(nullptr),
      explosionImage{ new QPixmap(QPixmap{(board_size == 3) ? EXPLOSION_3x3_PATH : EXPLOSION_4x4_PATH}.scaled(EXPLOSION_SIZE, EXPLOSION_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation))},
      handImage{ new QPixmap(QPixmap{HAND_PATH}.scaled(EFFECT_SIZE, EFFECT_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) },
      removeImage{ new QPixmap(QPixmap{REMOVE_PATH}.scaled(EFFECT_SIZE, EFFECT_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) },
      holeImage{ new QPixmap(QPixmap{HOLE_PATH}.scaled(EFFECT_SIZE, EFFECT_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) },
      m_board_size{board_size},
      m_cell_size{ static_cast<double>(EXPLOSION_SIZE) / board_size },
      active{false},
    m_zone{nullptr}
{
    setZValue(11);
    this->setAcceptHoverEvents(false);
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

QRectF Explosion::boundingRect() const {
    return QRectF(-EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, EXPLOSION_SIZE, EXPLOSION_SIZE);
}

void Explosion::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
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
QPointF Explosion::mapEffectToCard(const base::Coord& coord) {
    QPointF casted{ gui::utils::coordToQPointF(coord) };
    QPointF swapped{ casted.y(), casted.x() };
    QPointF translated{ swapped + ((m_board_size == 3) ? QPointF{-1, -1 } : QPointF{-double(3) / 2, -double(3) / 2}) };
    QPointF scaled{ m_cell_size * translated };

    QPointF center_offset{ -EFFECT_SIZE / 2, -EFFECT_SIZE / 2 };

    return QPointF{ scaled + center_offset };
}

void Explosion::setActive() {
    active = true;
    this->setFlags(QGraphicsItem::ItemIsMovable);
    
    this->update();
}

void Explosion::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    scene()->clearSelection();
    dragOffset = pos() - mapToScene(event->pos());
    lastCardPosition = pos();

    QGraphicsItem::mousePressEvent(event);
}

void Explosion::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (m_zone->contains(this->pos())) {
        m_zone->show();
    }
    else {
        m_zone->hide();
    }

    QPointF newPos = mapToScene(event->pos()) + dragOffset;

    QRectF sceneRect = scene()->sceneRect();
    newPos.setX(qBound(sceneRect.left(), newPos.x(), sceneRect.right()));
    newPos.setY(qBound(sceneRect.top(), newPos.y(), sceneRect.bottom()));

    QGraphicsItem::mouseMoveEvent(event);
}

void Explosion::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    double distance_to_vortex{ std::hypot(pos().x() - VORTEX_POS.x(), pos().y() - VORTEX_POS.y()) };
    if (distance_to_vortex < VERTEX_RANGE) {
        emit discard();
    }

    if (m_zone != nullptr && m_zone->contains(this->pos())) {
        emit apply();
    }

    QGraphicsItem::mouseReleaseEvent(event);
}

void Explosion::setTargetZone(TargetZone* zone) {
    m_zone = zone;
}

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

Vortex::Vortex() : image{new QPixmap(QPixmap{VORTEX_PATH}.scaled(VORTEX_SIZE, VORTEX_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation)) },
    angle{0} {
    setPos(VORTEX_POS);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Vortex::rotateVortex);
    timer->start(32);
}

void Vortex::rotateVortex() {
    angle -= 2;
    if (angle <= 360) {
        angle += 360;
    }
    update();
}

void Vortex::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
    painter->rotate(angle);
    painter->drawPixmap(-VORTEX_SIZE / 2, -VORTEX_SIZE / 2, *image);
    painter->restore();
}

QRectF Vortex::boundingRect() const {
    return QRectF(-VORTEX_SIZE / 2, -VORTEX_SIZE / 2, VORTEX_SIZE, VORTEX_SIZE);
}

