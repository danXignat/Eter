#include "items.h"

#include "settings.h"

/// -----------------------------------------------------------BOARD----------------------------------------------

BoardCell::BoardCell(const QPointF& pos)
    : QGraphicsRectItem(pos.x(), pos.y(), CARD_SIZE, CARD_SIZE) {
    QPen pen;
    pen.setColor(QColor(57, 255, 20));
    pen.setWidth(5);

    QBrush brush;
    brush.setColor(QColor(0, 255, 0, 127));
    brush.setStyle(Qt::SolidPattern);

    setPen(pen);
    setBrush(brush);
}

Card::Card(color::ColorType color, base::CombatCardType type, const QPointF& pos, uint16_t ID, QGraphicsItem* parent)
    : QGraphicsItem(parent),
    color           { color },
    type            { type },
    placed          { false },
    used            { false },
    is_illusion     { false },
    stack_move_event{ false },
    start_pos       { pos },
    m_ID            { ID },
    m_state         {CardState::DEFAULT} {

    QString image_path = QString(CARD_PATH)
        .arg(color == color::ColorType::RED ? "red" : "blue")
        .arg(combatCardToChar(type));
    QString backPath = QString(BACK_PATH)
        .arg(color == color::ColorType::RED ? "red" : "blue")
        .arg("back");

    cardImage = QPixmap(image_path);
    cardBack = QPixmap(backPath);

    red_x = QPixmap("../pictures/red_x.png")
        .scaled(CARD_SIZE - RED_X_PADDING, CARD_SIZE - RED_X_PADDING, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    setAcceptHoverEvents(true);
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

    if (is_illusion == false) {
        QPixmap scaledImage = cardImage.scaled(CARD_SIZE, CARD_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        painter->drawPixmap(-CARD_SIZE / 2, -CARD_SIZE / 2, scaledImage);
    }
    else{
        QPixmap scaledImage = cardBack.scaled(CARD_SIZE, CARD_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        painter->drawPixmap(-CARD_SIZE / 2, -CARD_SIZE / 2, scaledImage);
    }

    QPen pen;
    QBrush brush;
    pen.setWidth(2);

    switch (m_state)
    {
    case CardState::AVAILABLE:
        pen.setColor(QColor(0, 255, 0));
        brush.setStyle(Qt::NoBrush);
        break;

    case CardState::ABOUT_TO_REMOVE:
        pen.setColor(QColor(255, 0, 0));
        brush.setStyle(Qt::NoBrush);
        break;

    case CardState::RESTRICTED:
        brush.setColor(QColor(255, 0, 0, 127));
        brush.setStyle(Qt::SolidPattern);
        pen.setColor(QColor(255, 0, 0));
        break;

    case CardState::REMOVE:
        pen.setColor(QColor(255, 0, 0));
        brush.setStyle(Qt::NoBrush);
        painter->drawPixmap(-(CARD_SIZE - RED_X_PADDING) / 2, -(CARD_SIZE - RED_X_PADDING) / 2, red_x);
        break;

    default:
        pen = Qt::NoPen;
        brush = Qt::NoBrush;
        break;
    }

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(QRect{ -CARD_SIZE / 2, -CARD_SIZE / 2, CARD_SIZE, CARD_SIZE });
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

bool Card::inStackMoveEvent() const {
    return stack_move_event;
}
void Card::setStackMoveEvent(bool in_event) {
    stack_move_event = in_event;
}

void Card::setState(CardState state) {
    m_state = state;
    update();
    setAcceptHoverEvents(true);
}

void Card::setPlaced(bool placed) {
    this->placed = placed;
    this->setFlag(QGraphicsItem::ItemIsMovable, !placed);
    this->setFlag(QGraphicsItem::ItemIsSelectable, !placed);

    setAcceptedMouseButtons(Qt::LeftButton);
    this->update();
}

bool Card::isPlaced() const {
    return placed;
}
void Card::flipCard(){
    is_illusion = !is_illusion;
    update();
}
bool Card::isIllusion() {
    return is_illusion;
}
void Card::mousePressEvent(QGraphicsSceneMouseEvent* event) {

    switch (event->button()) {

    case Qt::LeftButton: {
        lastMousePosition = event->scenePos();
        lastCardPosition = pos();

        if (m_state == CardState::REMOVE) {
            emit clickedOnRemove(this);
        }
        else {
            emit cardStartMoving(this);
        }

        break;
    }

    case Qt::RightButton: {
        if (placed == false) {
            emit selectIllusion(this);
        }
        break;
    }

    default:
        break;
    }
    
    m_dragStart = event->scenePos();

    QGraphicsItem::mousePressEvent(event);
}

void Card::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QPointF delta = event->scenePos() - m_dragStart;

    emit isMoving(this);
    emit movingWithDelta(this, delta);

    setPos(pos() + delta);

    m_dragStart = event->scenePos();
    event->accept();
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        QGraphicsItem::mouseReleaseEvent(event);
        return;
    }

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
            if (!stack_move_event) {
                setZValue(1);
            }
            break;
        }

        auto* card = dynamic_cast<Card*>(item);
        if (card && card != this && !stack_move_event) {
            QPointF target_coord{ card->sceneBoundingRect().center() };

            setPos(target_coord);
            setZValue(card->zValue() + 1);
            break;
        }
    }

    emit cardStoppedMoving(this);
    emit cardAppend(this);

    qDebug() << "Card snapped to:" << pos();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    if (m_state == CardState::ABOUT_TO_REMOVE) {
        m_state = CardState::REMOVE;
        update();

        emit hoverRemoveEnter(this);
    }

    QGraphicsItem::hoverEnterEvent(event);
}
void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    if (m_state == CardState::REMOVE) {
        m_state = CardState::ABOUT_TO_REMOVE;
        update();

        emit hoverRemoveLeave(this);
    }

    QGraphicsItem::hoverLeaveEvent(event);
}
void Card::hoverMoveEvent(QGraphicsSceneHoverEvent* event) {

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

void Hole::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QList<QGraphicsItem*> itemsUnderCard = scene()->items(pos());

    for (QGraphicsItem* item : itemsUnderCard) {
        if (auto* cell = dynamic_cast<BoardCell*>(item)) {
            QPointF target_coord{ cell->sceneBoundingRect().center() };

            setPos(target_coord);
            setZValue(1);

            emit placed();

            break;
        }
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

Explosion::Explosion(const std::unordered_map<base::Coord, base::Effect, base::utils::CoordFunctor>& effects, uint16_t board_size)
    : QGraphicsItem(nullptr),
      explosionImage{ new QPixmap(QPixmap{(board_size == 3) ? EXPLOSION_3x3_PATH : EXPLOSION_4x4_PATH}.scaled(SPECIAL_CARD_SIZE, SPECIAL_CARD_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation))},
      handImage   { new QPixmap(QPixmap{HAND_PATH}.scaled(EFFECT_SIZE, EFFECT_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) },
      removeImage { new QPixmap(QPixmap{REMOVE_PATH}.scaled(EFFECT_SIZE, EFFECT_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) },
      holeImage   { new QPixmap(QPixmap{HOLE_PATH}.scaled(EFFECT_SIZE, EFFECT_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) },
      m_board_size{board_size},
      m_cell_size{ static_cast<double>(SPECIAL_CARD_SIZE) / board_size },
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
    return QRectF(-SPECIAL_CARD_SIZE / 2, -SPECIAL_CARD_SIZE / 2, SPECIAL_CARD_SIZE, SPECIAL_CARD_SIZE);
}

void Explosion::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(-SPECIAL_CARD_SIZE / 2, -SPECIAL_CARD_SIZE / 2, *explosionImage);

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

    painter->drawRect(QRect{ -SPECIAL_CARD_SIZE / 2, -SPECIAL_CARD_SIZE / 2, SPECIAL_CARD_SIZE, SPECIAL_CARD_SIZE });
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

    QGraphicsItem::mouseMoveEvent(event);
}

void Explosion::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    double distance_to_vortex{ std::hypot(pos().x() - VORTEX_POS.x(), pos().y() - VORTEX_POS.y()) };
    if (distance_to_vortex < VERTEX_RANGE) {
        emit discard();
    }

    if (m_zone != nullptr && m_zone->contains(this->pos())) {
        m_zone->hide();

        emit apply();
    }

    qDebug() << pos();

    QGraphicsItem::mouseReleaseEvent(event);
}

void Explosion::setTargetZone(TargetZone* zone) {
    m_zone = zone;
}

TargetZone::TargetZone() {
    setZValue(6);

    QBrush brush;
    QColor semiTransparentRed(255, 0, 0, 127);
    brush.setColor(semiTransparentRed);
    brush.setStyle(Qt::SolidPattern);

    setBrush(Qt::NoBrush);
    setPen(Qt::NoPen);
}

void TargetZone::setCorners(const QPointF& coord1, const QPointF& coord2) {
    m_point1 = coord1 - QPointF(CARD_SIZE, CARD_SIZE);
    m_point2 = coord2 + QPointF(CARD_SIZE, CARD_SIZE);

    qreal x = std::min(m_point1.x(), m_point2.x());
    qreal y = std::min(m_point1.y(), m_point2.y());
    qreal width = std::abs(m_point2.x() - m_point1.x());
    qreal height = std::abs(m_point2.y() - m_point1.y());

    setRect(x, y, width, height);
    update();
}

void TargetZone::show() {
    if (brush().style() == Qt::SolidPattern) {
        return;
    }

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

///-------------------------------------------MAGE-----------------------------------------------------------------------------------

MageCard::MageCard(base::MageTypeAbility type, const QString& description, color::ColorType color, QGraphicsItem* parent) : QGraphicsItem(parent),
type{type},
m_color(color),
m_description{description} {
    card_image = QPixmap(QString(MAGE_PATH).arg(static_cast<int>(type)))
        .scaled(SPECIAL_CARD_SIZE, SPECIAL_CARD_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    info_icon = QPixmap("../pictures/info_icon.png")
        .scaled(INFO_ICON_SIZE, INFO_ICON_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    info_icon_tapped = QPixmap("../pictures/info_icon_tapped.png")
        .scaled(INFO_ICON_SIZE, INFO_ICON_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    info_icon_rect = QRectF(
        SPECIAL_CARD_SIZE / 2 - INFO_ICON_SIZE - 5,  // 5 pixels padding from edge
        SPECIAL_CARD_SIZE / 2 - INFO_ICON_SIZE - 5,
        INFO_ICON_SIZE,
        INFO_ICON_SIZE
    );

    setAcceptHoverEvents(true);
    setZValue(11);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    setPos(MAGE_POS);
}

void MageCard::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    painter->drawPixmap(-SPECIAL_CARD_SIZE / 2, -SPECIAL_CARD_SIZE / 2, card_image);

    painter->drawPixmap(info_icon_rect.toRect(),
        is_hovering_info ? info_icon_tapped : info_icon);
}

QRectF MageCard::boundingRect() const {
    return QRectF(-SPECIAL_CARD_SIZE / 2, -SPECIAL_CARD_SIZE / 2, SPECIAL_CARD_SIZE, SPECIAL_CARD_SIZE);
}

void MageCard::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QPointF pos = event->pos();
    if (info_icon_rect.contains(pos)) {
        showDescription();
        event->accept();
        return;
    }

    QGraphicsItem::mousePressEvent(event);
}

void MageCard::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (m_zone->contains(this->pos())) {
        m_zone->show();
    }
    else {
        m_zone->hide();
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void MageCard::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (m_zone->contains(this->pos())) {
        m_zone->hide();

        emit applyMageCard(this);
    }

    QGraphicsItem::mouseReleaseEvent(event);
}

void MageCard::hoverMoveEvent(QGraphicsSceneHoverEvent* event) {
    bool wasHovering = is_hovering_info;
    is_hovering_info = info_icon_rect.contains(event->pos());

    if (wasHovering != is_hovering_info) {
        update(info_icon_rect); // Only redraw the icon area
    }
}

void MageCard::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    Q_UNUSED(event);
    if (is_hovering_info) {
        is_hovering_info = false;
        update(info_icon_rect);
    }
}

void MageCard::showDescription() {
    QPoint screenPos = scene()->views().first()->mapToGlobal(
        scene()->views().first()->mapFromScene(pos() + QPointF(0, 70))
    );

    DescriptionPopup* popup = new DescriptionPopup{ m_description };
    popup->showAtPosition(screenPos);
}

void MageCard::setTargetZone(TargetZone* zone) {
    m_zone = zone;
}

void MageCard::setUsed(bool is_used) {
    m_is_used = is_used;

    if (is_used) {
        hide();
    }
    else {
        show();
    }
}

bool MageCard::isUsed() const {
    return m_is_used;
}

color::ColorType MageCard::getColor() const {
    return m_color;
}

base::MageTypeAbility MageCard::getTypeAbility() const {
    return type;
}