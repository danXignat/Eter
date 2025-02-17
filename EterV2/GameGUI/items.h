#pragma once
#include "utils.h"
#include "controls.h"
#include "qt_includes.h"
#include "..\EterV2\CombatCard.h"
#include "..\EterV2\ExplosionService.h"

enum class CardState : uint16_t {
    DEFAULT,
    AVAILABLE,
    RESTRICTED,
    REMOVE,
    ABOUT_TO_REMOVE
};

/// -----------------------------------------------------------BOARD----------------------------------------------

class BoardCell :public QGraphicsRectItem {

public:
    BoardCell(const QPointF& pos);
};

class Card : public QObject, public QGraphicsItem {
    Q_OBJECT
signals:
    void cardAppend         (Card* card);
    void cardStartMoving    (Card* card);
    void isMoving           (Card* card);
    void cardStoppedMoving  (Card* card);
    void selectIllusion     (Card* card);
    void clickedOnRemove    (Card* card);
    void hoverRemoveEnter   (Card* card);
    void hoverRemoveLeave   (Card* card);
    void movingWithDelta    (Card* card, QPointF delta);

public:
    explicit Card(color::ColorType, base::CombatCardType type, const QPointF& pos, uint16_t ID, QGraphicsItem* parent = nullptr);

    uint16_t getID() const;
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    void moveCardBack();

    base::CombatCardType getType() const;
    color::ColorType getColor() const;
    bool isPlaced() const;
    bool isUsed() const;
    bool inStackMoveEvent() const;
    void setStackMoveEvent(bool in_event);
    void setUsed(bool used);
    void setPlaced(bool placed);
    void setState(CardState state);
    void flipCard();
    bool isIllusion();


protected:
    
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    

private:
    QPixmap cardImage;
    QPixmap cardBack;
    QPixmap red_x;

    QPointF start_pos;
    QPointF lastMousePosition;
    QPointF lastCardPosition;
    QPointF m_dragStart;

    uint16_t m_ID;
    color::ColorType color;
    base::CombatCardType type;
    CardState m_state;

    bool placed;
    bool used;
    bool is_illusion;
    bool stack_move_event;
};

///----------------------------------------------------------EXPLOSION----------------------------------------------------
class Hole : public QObject, public QGraphicsItem {
    Q_OBJECT
signals:
    void placed();

public:
    Hole(const QPointF& pos);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QPixmap* image;
};

class TargetZone :public QGraphicsRectItem {

public:
    TargetZone();
    void show();
    void hide();
    void setCorners(const QPointF& coord1, const QPointF& coord2);

private:
    QPointF m_point1;
    QPointF m_point2;
};

class Vortex : public QObject, public QGraphicsItem {
    Q_OBJECT

public:
    Vortex();

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private slots:
    void rotateVortex();

private:
    QPixmap* image;
    qreal angle;
};


class Explosion : public QObject, public QGraphicsItem {
    Q_OBJECT
signals:
    void leftRotate();
    void rightRotate();
    void activate();
    void discard();
    void apply();

public:
    explicit Explosion(const std::unordered_map<base::Coord, base::Effect, base::utils::CoordFunctor>&, uint16_t);

    void setActive();
    void setTargetZone(TargetZone* zone);

   // void advance(int phase) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QPointF mapEffectToCard(const base::Coord& coord);

private:
    QPixmap* explosionImage;
    QPixmap* handImage;
    QPixmap* removeImage;
    QPixmap* holeImage;

    TargetZone* m_zone;

    bool active;
    uint16_t m_board_size;
    double m_cell_size;
    QHash<QPointF, QPixmap*> m_effects;

    QPointF dragOffset;
    QPointF lastCardPosition;
};

///-------------------------------------------MAGE-----------------------------------------------------------------------------------

class MageCard : public QObject, public QGraphicsItem {
    Q_OBJECT
signals:
    void applyMageCard(MageCard* card);

public:
    explicit MageCard(base::MageTypeAbility type, const QString& description, color::ColorType color, QGraphicsItem* parent = nullptr);


    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;

    bool isUsed() const;
    
    color::ColorType getColor() const;
    base::MageTypeAbility getTypeAbility() const;
    void setTargetZone(TargetZone* zone);
    void setUsed(bool is_used);

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent (QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    void showDescription();

private:
    QPixmap card_image;
    QPixmap info_icon;
    QPixmap info_icon_tapped;
    bool is_hovering_info = false;
    QRectF info_icon_rect;

    QPointF start_pos;
    QPointF last_mouse_position;
    QPointF last_card_position;

    TargetZone* m_zone;

    QString m_description;
    color::ColorType m_color;
    base::MageTypeAbility type;

    bool m_is_used;
};

///---------------------------------------ELEMENTAL---------------------------------

///-----------------------------------------TOURNAMENT---------------------------------

class Arena : public QGraphicsPixmapItem {
public:
    Arena(base::GameSizeType mode, QGraphicsItem* parent = nullptr)
        : QGraphicsPixmapItem(parent) {

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

    QSizeF getSize() const {
        return pixmap().size();
    }

    QPointF getPosition() const {
        return pos();
    }

    void setArenaPosition(const QPointF& newPos) {
        setPos(newPos - QPointF(pixmap().width() / 2.0, pixmap().height() / 2.0));
    }

    void setArenaSize(const QSizeF& newSize) {
        QPixmap scaledPixmap = originalPixmap.scaled(
            newSize.toSize(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );
        setPixmap(scaledPixmap);
        // Maintain center position
        setArenaPosition(pos() + QPointF(pixmap().width() / 2.0, pixmap().height() / 2.0));
    }

private:
    QPixmap originalPixmap;
};