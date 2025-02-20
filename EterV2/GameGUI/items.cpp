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

    hand = QPixmap("../pictures/hand.png")
        .scaled(CARD_SIZE - RED_X_PADDING, CARD_SIZE - RED_X_PADDING, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    check = QPixmap("../pictures/check.png")
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

    case CardState::ABOUT_TO_CHECK:
        pen.setColor(QColor(0, 255, 0));
        brush.setStyle(Qt::NoBrush);
        break;

    case CardState::CHECK:
        pen.setColor(QColor(0, 255, 0));
        brush.setStyle(Qt::NoBrush);
        painter->drawPixmap(-(CARD_SIZE - RED_X_PADDING) / 2, -(CARD_SIZE - RED_X_PADDING) / 2, check);
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

    case CardState::ABOUT_TO_HAND:
        pen.setColor(QColor(255, 255, 0));
        brush.setStyle(Qt::NoBrush);
        break;

    case CardState::HAND:
        pen.setColor(QColor(255, 255, 0));
        brush.setStyle(Qt::NoBrush);
        painter->drawPixmap(-(CARD_SIZE - RED_X_PADDING) / 2, -(CARD_SIZE - RED_X_PADDING) / 2, hand);
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

        if (m_state == CardState::REMOVE || m_state == CardState::HAND) {
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

    if (m_state == CardState::ABOUT_TO_HAND) {
        m_state = CardState::HAND;
        update();

        emit hoverRemoveEnter(this);
    }

    if (m_state == CardState::ABOUT_TO_CHECK) {
        m_state = CardState::CHECK;
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

    if (m_state == CardState::HAND) {
        m_state = CardState::ABOUT_TO_HAND;
        update();

        emit hoverRemoveLeave(this);
    }

    if (m_state == CardState::CHECK) {
        m_state = CardState::ABOUT_TO_CHECK;
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

///-------------------------

PowerCard::PowerCard(uint16_t id, base::PowerCardType type, const QString& description, QGraphicsItem* parent) : QGraphicsItem(parent),
type{ type },
m_id{id},
m_description{ description } {
    card_image = QPixmap(QString("../pictures/elemental/") + base::typeToStrings(type).data() + ".png")
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

void PowerCard::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(-SPECIAL_CARD_SIZE / 2, -SPECIAL_CARD_SIZE / 2, card_image);

    painter->drawPixmap(info_icon_rect.toRect(),
        is_hovering_info ? info_icon_tapped : info_icon);
}

QRectF PowerCard::boundingRect() const {
    return QRectF(-SPECIAL_CARD_SIZE / 2, -SPECIAL_CARD_SIZE / 2, SPECIAL_CARD_SIZE, SPECIAL_CARD_SIZE);
}

void PowerCard::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QPointF pos = event->pos();
    if (info_icon_rect.contains(pos)) {
        showDescription();
        event->accept();
        return;
    }

    QGraphicsItem::mousePressEvent(event);
}

uint16_t  PowerCard::getId()const {
    return m_id;
}

void PowerCard::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (m_zone->contains(this->pos())) {
        m_zone->show();
    }
    else {
        m_zone->hide();
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void PowerCard::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (m_zone->contains(this->pos())) {
        m_zone->hide();

        emit applyPowerCard(this);
    }

    qDebug() << pos();

    QGraphicsItem::mouseReleaseEvent(event);
}

void PowerCard::hoverMoveEvent(QGraphicsSceneHoverEvent* event) {
    bool wasHovering = is_hovering_info;
    is_hovering_info = info_icon_rect.contains(event->pos());

    if (wasHovering != is_hovering_info) {
        update(info_icon_rect); // Only redraw the icon area
    }
}

void PowerCard::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    Q_UNUSED(event);
    if (is_hovering_info) {
        is_hovering_info = false;
        update(info_icon_rect);
    }
}

void PowerCard::showDescription() {
    QPoint screenPos = scene()->views().first()->mapToGlobal(
        scene()->views().first()->mapFromScene(pos() + QPointF(0, 70))
    );

    DescriptionPopup* popup = new DescriptionPopup{ m_description };
    popup->showAtPosition(screenPos);
}

void PowerCard::setTargetZone(TargetZone* zone) {
    m_zone = zone;
}

void PowerCard::setUsed(bool is_used) {
    m_is_used = is_used;

    if (is_used) {
        hide();
    }
    else {
        show();
    }
}

bool PowerCard::isUsed() const {
    return m_is_used;
}

base::PowerCardType PowerCard::getTypeAbility() const {
    return type;
}

void PowerCard::setStartPoint(const QPointF& point) {
    start_point = point;
}

void PowerCard::goBack() {
    setPos(start_point);
}


///-----------------------------------------TOURNAMENT---------------------------------

Arena::Arena(base::GameSizeType mode, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
    game_size{mode},
    m_board_size{ static_cast<int>(mode) },
    m_cell_size{ static_cast<double>(ARENA_SIZE) / static_cast<int>(mode) } {
    QString texturePath = "../pictures/tournament/arena_";
    texturePath += (mode == base::GameSizeType::SMALL) ? "small.png" : "big.png";

    originalPixmap.load(texturePath);
    if (originalPixmap.isNull()) {
        qDebug() << "Failed to load arena texture:" << texturePath;
        return;
    }

    QPixmap scaledPixmap = originalPixmap.scaled(
        ARENA_SIZE, ARENA_SIZE,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );

    setPixmap(scaledPixmap);
    setPos(ARENA_POS - QPointF(scaledPixmap.width() / 2.0, scaledPixmap.height() / 2.0));
    setZValue(0);
}

QSizeF Arena::getSize() const {
    return pixmap().size();
}

QPointF Arena::getPosition() const {
    return pos();
}

void Arena::setArenaPosition(const QPointF& newPos) {
    setPos(newPos - QPointF(pixmap().width() / 2.0, pixmap().height() / 2.0));
}

void Arena::setArenaSize(const QSizeF& newSize) {
    QPixmap scaledPixmap = originalPixmap.scaled(
        newSize.toSize(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );

    setPixmap(scaledPixmap);
    // Maintain center position
    setArenaPosition(pos() + QPointF(pixmap().width() / 2.0, pixmap().height() / 2.0));
}

QPointF Arena::mapEffectToCard(const QPointF& pos) {
    QPointF swapped{ pos.y(), pos.x() };
    QPointF translated{ swapped + ((m_board_size == 3) ? QPointF{-1, -1 } : QPointF{-double(3) / 2, -double(3) / 2}) };
    QPointF scaled{ MARKER_SIZE * translated };

    return QPointF{ scaled };
}

/// ----------------------------------------------------------------------

Marker::Marker(color::ColorType color, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , m_color(color)
{
    QString imagePath;
    if (color == color::ColorType::RED || color == color::ColorType::BLUE) {
        imagePath = (color == color::ColorType::RED)
            ? QString("../pictures/tournament/marker_red.png")
            : QString("../pictures/tournament/marker_blue.png");
    }
    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        qWarning() << "Failed to load marker image:" << imagePath;
    }

    // Scale the pixmap to MARKER_SIZE while maintaining aspect ratio
    pixmap = pixmap.scaled(MARKER_SIZE, MARKER_SIZE,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);

    setPixmap(pixmap);

    // Make the item moveable
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

color::ColorType Marker::getColor() const {
    return m_color;
}

void Marker::setPos(const QPointF& pos) {
    QGraphicsPixmapItem::setPos(pos - QPointF(MARKER_SIZE / 2.0, MARKER_SIZE / 2.0));
}

///-------------------------------------------------------------------

VictoryScreen::VictoryScreen(const QString& redPlayerName, const QString& bluePlayerName, QWidget* parent)
    : QWidget(parent)
    , redName(redPlayerName)
    , blueName(bluePlayerName)
{
    setupUI();
    hide();
}

void VictoryScreen::setupUI() {
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Background with gradient
    backgroundLabel = new QLabel(this);
    backgroundLabel->setStyleSheet(
        "QLabel {"
        "   background-color: rgba(0, 0, 0, 180);"  // Changed to semi-transparent black
        "}"
    );
    backgroundLabel->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Victory Image (keep your existing image handling)
    victoryLabel = new QLabel(this);
    victoryLabel->setFixedSize(VICTORY_IMAGE_WIDTH, VICTORY_IMAGE_HEIGHT);
    int imageX = (WINDOW_WIDTH - VICTORY_IMAGE_WIDTH) / 2;
    int imageY = (WINDOW_HEIGHT - VICTORY_IMAGE_HEIGHT) / 3;  // Moved up slightly
    victoryLabel->move(imageX, imageY);

    // Score Container
    QWidget* scoreContainer = new QWidget(this);
    scoreContainer->setFixedSize(800, 200);  // Adjust size as needed

    // Layout for score container
    QHBoxLayout* scoreLayout = new QHBoxLayout(scoreContainer);
    scoreLayout->setSpacing(40);
    scoreLayout->setContentsMargins(20, 20, 20, 20);

    // Red Score Card
    QWidget* redCard = new QWidget;
    redCard->setFixedSize(300, 160);
    redCard->setStyleSheet(
        "QWidget {"
        "   background-color: rgba(31, 41, 55, 255);"
        "   border: 2px solid #ef4444;"
        "   border-radius: 12px;"
        "}"
    );

    QVBoxLayout* redCardLayout = new QVBoxLayout(redCard);
    redCardLayout->setAlignment(Qt::AlignCenter);

    redScoreLabel = new QLabel;
    redScoreLabel->setFont(QFont("Arial", 24, QFont::Bold));
    redScoreLabel->setStyleSheet(
        "QLabel {"
        "   color: #ef4444;"
        "   border: none;"
        "   padding: 10px;"
        "}"
    );
    redScoreLabel->setAlignment(Qt::AlignCenter);

    QLabel* redPoints = new QLabel("points");
    redPoints->setFont(QFont("Arial", 14));
    redPoints->setStyleSheet("color: #f87171; border: none;");
    redPoints->setAlignment(Qt::AlignCenter);

    redCardLayout->addWidget(redScoreLabel);
    redCardLayout->addWidget(redPoints);

    QWidget* dividerContainer = new QWidget;
    QVBoxLayout* dividerLayout = new QVBoxLayout(dividerContainer);
    dividerContainer->setFixedWidth(40);  // Fixed width
    dividerLayout->setSpacing(0);  // Remove extra spacing
    dividerLayout->setContentsMargins(0, 0, 0, 0);  // Remove margins

    QLabel* vsLabel = new QLabel("VS");
    vsLabel->setFixedHeight(40);
    vsLabel->setFont(QFont("Arial", 20, QFont::Bold));
    vsLabel->setStyleSheet("color: #9ca3af;");
    vsLabel->setAlignment(Qt::AlignCenter);

    dividerLayout->addWidget(vsLabel);
    dividerLayout->setAlignment(Qt::AlignCenter);

    // Blue Score Card
    QWidget* blueCard = new QWidget;
    blueCard->setFixedSize(300, 160);
    blueCard->setStyleSheet(
        "QWidget {"
        "   background-color: rgba(31, 41, 55, 255);"
        "   border: 2px solid #3b82f6;"
        "   border-radius: 12px;"
        "}"
    );

    QVBoxLayout* blueCardLayout = new QVBoxLayout(blueCard);
    blueCardLayout->setAlignment(Qt::AlignCenter);

    blueScoreLabel = new QLabel;
    blueScoreLabel->setFont(QFont("Arial", 24, QFont::Bold));
    blueScoreLabel->setStyleSheet(
        "QLabel {"
        "   color: #3b82f6;"
        "   border: none;"
        "   padding: 10px;"
        "}"
    );
    blueScoreLabel->setAlignment(Qt::AlignCenter);

    QLabel* bluePoints = new QLabel("points");
    bluePoints->setFont(QFont("Arial", 14));
    bluePoints->setStyleSheet("color: #60a5fa; border: none;");
    bluePoints->setAlignment(Qt::AlignCenter);

    blueCardLayout->addWidget(blueScoreLabel);
    blueCardLayout->addWidget(bluePoints);

    // Add cards and divider to score layout
    scoreLayout->addWidget(redCard);
    scoreLayout->addWidget(dividerContainer);
    scoreLayout->addWidget(blueCard);

    // Position score container
    int scoreContainerY = imageY + VICTORY_IMAGE_HEIGHT + 40;
    scoreContainer->move((WINDOW_WIDTH - scoreContainer->width()) / 2, scoreContainerY);

    // Buttons
    nextButton = new NextButton(this);
    nextButton->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    nextButton->move(WINDOW_WIDTH - 385, WINDOW_HEIGHT - 122);
    nextButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4f46e5;"
        "   color: white;"
        "   border-radius: 6px;"
        "   padding: 8px 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #4338ca;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3730a3;"
        "}"
    );

    mainMenuButton = new QPushButton("Main Menu", this);
    mainMenuButton->setFixedSize(365, 92);
    mainMenuButton->move((WINDOW_WIDTH - BUTTON_WIDTH) / 2, WINDOW_HEIGHT - 122);
    mainMenuButton->setStyleSheet(buttonStyle);
    mainMenuButton->hide();

    QGraphicsDropShadowEffect* redShadow = new QGraphicsDropShadowEffect(this);
    redShadow->setBlurRadius(20);
    redShadow->setColor(QColor(239, 68, 68, 75));
    redShadow->setOffset(0, 0);
    redCard->setGraphicsEffect(redShadow);

    QGraphicsDropShadowEffect* blueShadow = new QGraphicsDropShadowEffect(this);
    blueShadow->setBlurRadius(20);
    blueShadow->setColor(QColor(59, 130, 246, 75));
    blueShadow->setOffset(0, 0);
    blueCard->setGraphicsEffect(blueShadow);

    connect(nextButton, &QPushButton::clicked, this, [this]() {
        hide();
        emit nextRoundRequested();
        });

    connect(mainMenuButton, &QPushButton::clicked, this, [this]() {
        hide();
        emit mainMenuRequested();
        });
}

void VictoryScreen::showVictory(color::ColorType color, int redScore, int blueScore) {
    QString imagePath = (color == color::ColorType::RED)
        ? "../pictures/win/red_victory.png"
        : "../pictures/win/blue_victory.png";

    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image:" << imagePath;
        return;
    }

    QPixmap scaledPixmap = pixmap.scaled(
        VICTORY_IMAGE_WIDTH,
        VICTORY_IMAGE_HEIGHT,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );

    victoryLabel->setPixmap(scaledPixmap);

    if (color == color::ColorType::RED) {
        redScore++;
    }
    else {
        blueScore++;
    }

    redScoreLabel->setText(QString("%1: %2").arg(redName).arg(redScore));
    blueScoreLabel->setText(QString("%1: %2").arg(blueName).arg(blueScore));

    show();
    raise();
}

void VictoryScreen::switchToMainMenu() {
    nextButton->hide();
    mainMenuButton->show();
}

QString VictoryScreen::buttonStyle{
    "QPushButton {"
    "    font-family: 'Luminari', 'Trattatello', 'Magic School One';"
    "    font-size: 24px;"
    "    color: #00A800;"
    "    font-weight: bold;"
    "    padding: 8px 15px;"
    "    background-color: rgba(0, 0, 0, 0.7);"
    "    border: 1px solid rgba(0, 168, 0, 0.3);"
    "    border-radius: 8px;"
    "    min-width: 120px;"  // Ensures minimum button width
    "    min-height: 45px;"  // Ensures minimum button height
    "}"
    "QPushButton:hover {"
    "    color: #00FF00;"
    "    background-color: rgba(0, 0, 0, 0.8);"
    "    border: 1px solid rgba(0, 168, 0, 0.6);"
    "}"
    "QPushButton:pressed {"
    "    color: #008000;"
    "    background-color: rgba(0, 0, 0, 0.9);"
    "    border: 2px solid #00A800;"
    "    padding: 9px 14px;"  // Adjust padding to prevent size change when pressed
    "}"
    "QPushButton:disabled {"
    "    color: rgba(0, 168, 0, 0.4);"
    "    background-color: rgba(0, 0, 0, 0.5);"
    "    border: 1px solid rgba(0, 168, 0, 0.1);"
    "}"
};

///-------------------------------------------------------

VisualTimer::VisualTimer(const QString& playerName, int initialSeconds, color::ColorType color, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , totalSeconds(initialSeconds)
    , currentSeconds(initialSeconds)
    , isActive(false)
    , colorType(color)
{
    // Set the primary color based on ColorType
    primaryColor = (colorType == color::ColorType::RED) ? "#f44336" : "#2196F3";

    createWidgets();
    nameLabel->setText(playerName);
    updateDisplay();

    timer = new QTimer;
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &VisualTimer::updateTimer);
}

void VisualTimer::createWidgets() {
    progressBar = new QProgressBar;
    progressBar->setMinimum(0);
    progressBar->setMaximum(totalSeconds);
    progressBar->setValue(currentSeconds);
    progressBar->setTextVisible(false);
    progressBar->setFixedSize(WIDTH, 20);

    QString progressStyle = QString(
        "QProgressBar {"
        "   border: 2px solid grey;"
        "   border-radius: 5px;"
        "   background-color: #f0f0f0;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: %1;"
        "   width: 1px;"
        "}"
    ).arg(primaryColor);

    progressBar->setStyleSheet(progressStyle);

    timeLabel = new QLabel;
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("QLabel { font-size: 24pt; font-weight: bold; }");
    timeLabel->setFixedSize(WIDTH, 40);

    nameLabel = new QLabel;
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet("QLabel { font-size: 16pt; font-weight: bold; }");
    nameLabel->setFixedSize(WIDTH, 30);

    progressBarProxy = new QGraphicsProxyWidget(this);
    progressBarProxy->setWidget(progressBar);
    progressBarProxy->setPos(0, 70);

    timeLabelProxy = new QGraphicsProxyWidget(this);
    timeLabelProxy->setWidget(timeLabel);
    timeLabelProxy->setPos(0, 30);

    nameLabelProxy = new QGraphicsProxyWidget(this);
    nameLabelProxy->setWidget(nameLabel);
    nameLabelProxy->setPos(0, 0);
}

QRectF VisualTimer::boundingRect() const {
    return QRectF(0, 0, WIDTH, HEIGHT);
}

void VisualTimer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void VisualTimer::updateTimer() {
    if (currentSeconds > 0) {
        currentSeconds--;
        progressBar->setValue(currentSeconds);

        updateDisplay();

        if (currentSeconds == 0) {
            pause();
            emit timedUp(colorType);
        }
    }
}

void VisualTimer::updateDisplay() {
    timeLabel->setText(formatTime(currentSeconds));
}

QString VisualTimer::formatTime(int seconds) const {
    int minutes = seconds / 60;
    int remainingSeconds = seconds % 60;
    return QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(remainingSeconds, 2, 10, QChar('0'));
}

void VisualTimer::setTotalTime(int seconds) {
    totalSeconds = seconds;
    currentSeconds = seconds;
    progressBar->setMaximum(totalSeconds);
    progressBar->setValue(currentSeconds);
    updateDisplay();
}

void VisualTimer::start() {
    if (currentSeconds > 0) {
        timer->start();
        isActive = true;
        nameLabel->setStyleSheet(QString("QLabel { font-size: 16pt; font-weight: bold; color: %1; }").arg(primaryColor));

        QString progressStyle = QString(
            "QProgressBar {"
            "   border: 2px solid grey;"
            "   border-radius: 5px;"
            "   background-color: #f0f0f0;"
            "}"
            "QProgressBar::chunk {"
            "   background-color: %1;"
            "   width: 1px;"
            "}"
        ).arg(primaryColor);

        progressBar->setStyleSheet(progressStyle);
    }
}

void VisualTimer::pause() {
    timer->stop();
    isActive = false;
    nameLabel->setStyleSheet("QLabel { font-size: 16pt; font-weight: bold; color: black; }");
    progressBar->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid grey;"
        "   border-radius: 5px;"
        "   background-color: #f0f0f0;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: grey;"
        "   width: 1px;"
        "}"
    );
}

void VisualTimer::reset() {
    pause();
    currentSeconds = totalSeconds;
    progressBar->setValue(currentSeconds);
    updateDisplay();
}

///-----------------------------------------

Arrow::Arrow(Direction direction, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , m_direction(direction)
    , m_isPressed(false)
    , m_isHovered(false)
{
    setAcceptHoverEvents(true);
    QString imagePath;
    switch (m_direction) {
    case LEFT:
        imagePath = "../pictures/left_arrow.png";
        break;
    case RIGHT:
        imagePath = "../pictures/right_arrow.png";
        break;
    case UP:
        imagePath = "../pictures/up_arrow.png";
        break;
    case DOWN:
        imagePath = "../pictures/down_arrow.png";
        break;
    }
    m_pixmap = QPixmap(imagePath).scaled(
        CARD_SIZE, CARD_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation
    );
    update();
}

QRectF Arrow::boundingRect() const
{
    return QRectF(-CARD_SIZE / 2, -CARD_SIZE / 2, CARD_SIZE, CARD_SIZE);
}

void Arrow::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setOpacity(m_isHovered ? 0.7 : 1.0);
    painter->drawPixmap(-CARD_SIZE / 2 + 2, -CARD_SIZE / 2 + 2, m_pixmap);
}

void Arrow::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPressed = true;
        update();

        emit clicked();
    }
    QGraphicsItem::mousePressEvent(event);
}

void Arrow::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_isPressed) {
        m_isPressed = false;

        update();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void Arrow::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    m_isHovered = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void Arrow::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    m_isHovered = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

/// <summary>
/// 
/// </summary>
/// <param name="startPoint"></param>
/// <param name="parent"></param>

ArrowItem::ArrowItem(const QPointF& startPoint, QGraphicsItem* parent)
    : QGraphicsLineItem(parent)
    , m_isDrawing(true)
    , m_arrowSize(10.0)
{
    setLine(QLineF(startPoint, startPoint));
    setFlags(ItemIsSelectable | ItemIsMovable);
    QPen thickPen(QColor(0, 170, 0));
    thickPen.setWidth(4);  // Increase this number to make it even thicker
    setPen(thickPen);
}

void ArrowItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QGraphicsLineItem::paint(painter, option, widget);

    QLineF mainLine = line();
    if (mainLine.length() > 0) {
        double angle = std::atan2(-mainLine.dy(), mainLine.dx());

        QPointF arrowP1 = mainLine.p2() - QPointF(sin(angle + M_PI / 3) * m_arrowSize,
            cos(angle + M_PI / 3) * m_arrowSize);
        QPointF arrowP2 = mainLine.p2() - QPointF(sin(angle + M_PI - M_PI / 3) * m_arrowSize,
            cos(angle + M_PI - M_PI / 3) * m_arrowSize);

        painter->setBrush(QColor(0, 170, 0));  // Strong green for arrow head
        painter->drawPolygon(QPolygonF() << mainLine.p2() << arrowP1 << arrowP2);
    }
}
