#pragma once
#include "qwebengineview.h"
#include "qt_includes.h"
#include "..\EterV2\GameModeFactory.h"
#include "items.h"
#include "utils.h"

namespace std {
    template<>
    struct hash<QPointF> {
        size_t operator()(const QPointF& point) const {
            return qHashMulti(0, point.x(), point.y());
        }
    };
}

class GameView : public QGraphicsView {
	Q_OBJECT

signals:
    void cardAppend(Card* card);

public:
	GameView(const QString& name_red, const QString& name_blue, QWidget* parent = nullptr);

    void drawPlayerCards(const base::Player& player, QPointF start_point);
    void drawAvailablePositions(const base::Board& board);
    void setDeckVisible(color::ColorType color, bool visible);
    void showWin(color::ColorType color);

    void _initWebView();
private:
    void _drawSquareAt(QPointF pos);
    void _initLabels(const QString& name_red, const QString& name_blue);
    Card* _createCardAt(color::ColorType color, base::CombatCardType type, QPointF pos);
    
private:
    QWebEngineView* webView;
    QVBoxLayout* layout;
    QLabel* infoLabel;
    QLabel* red_name_label;
    QLabel* blue_name_label;
    QLabel* won_label;

    QGraphicsScene* scene;

    QList<Card*> red_deck;
    QList<Card*> blue_deck;
    QHash<QPointF, QGraphicsRectItem*> m_available_cells;
};

class GameController : public QObject{
	Q_OBJECT

public:
    GameController(QWidget* parent,
        const std::string& mode,
        const QString& name_red, const QString& name_blue);

private:
    base::GameModePtr model;
    GameView* view;

public slots:
    void onCardAppend(Card* card);
};

