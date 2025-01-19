#pragma once
#include "utils.h"
#include "qt_includes.h"
#include "..\EterV2\CombatCard.h"
#include "..\EterV2\ExplosionService.h"

/// -----------------------------------------------------------BOARD----------------------------------------------

class BoardCell :public QGraphicsRectItem {

public:
    BoardCell(const QPointF& pos);
};

class Card : public QObject, public QGraphicsItem {
    Q_OBJECT
signals:
    void cardAppend(Card* card);

public:
    explicit Card(color::ColorType, base::CombatCardType, const QString& imagePath, const QPointF& pos, uint16_t ID, QGraphicsItem* parent = nullptr);

    uint16_t getID() const;
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    void moveCardBack();

    base::CombatCardType getType() const;
    color::ColorType getColor() const;
    bool isPlaced() const;
    bool isUsed() const;
    void setUsed(bool used);
    void setPlaced(bool placed);

    void setPlaced();
    void flipCard();
    bool isFaceUp();
protected:
    
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    uint16_t m_ID;
    QPixmap cardImage;
    QPointF start_pos;
    QPixmap cardBack;
    QPointF lastMousePosition;
    QPointF lastCardPosition;
    color::ColorType color;
    base::CombatCardType type;
    bool placed;
    bool used;

    bool faceUp;
};

///----------------------------------------------------------EXPLOSION----------------------------------------------------
class Hole : public QObject, public QGraphicsItem {
    Q_OBJECT

public:
    Hole(const QPointF& pos);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
    QPixmap* image;
};

class TargetZone :public QGraphicsRectItem {

public:
    TargetZone(QPointF coord1, QPointF coord2);
    void show();
    void hide();
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

