#pragma once
#include "utils.h"
#include "qt_includes.h"
#include "..\EterV2\CombatCard.h"
#include "..\EterV2\ExplosionCard.h"

class Card : public QObject, public QGraphicsItem {
    Q_OBJECT
signals:
    void cardAppend(Card* card);

public:
    explicit Card(color::ColorType, base::CombatCardType, const QString& imagePath, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    void moveCardBack();

    base::CombatCardType getType() const;
    color::ColorType getColor() const;
    bool isPlaced() const;
    void setPlaced();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QPixmap cardImage;
    QPointF lastMousePosition;
    QPointF lastCardPosition;
    color::ColorType color;
    base::CombatCardType type;
    bool placed;

};

class TargetZone : public QGraphicsRectItem {
public:
    TargetZone(QPointF coord1, QPointF coord2);
    void show();
    void hide();
};

class ExplosionView : public QObject, public QGraphicsItem {
    Q_OBJECT
signals:
    void leftRotate();
    void rightRotate();
    void activate();

public:
    explicit ExplosionView(const std::unordered_map<base::Coord, base::Effect, base::utils::CoordFunctor>&, uint16_t);

    void setActive();
    void setTargetZone(TargetZone* zone);

   // void advance(int phase) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;


protected:
    void keyPressEvent(QKeyEvent* event) override;
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

