#pragma once
#include "qt_includes.h"
#include "..\EterV2\CombatCard.h"

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