#pragma once
//#include "qwebengineview.h"
#include "qt_includes.h"
#include "..\EterV2\GameModeFactory.h"
#include "items.h"
#include "utils.h"

class GameView : public QGraphicsView {
	Q_OBJECT

signals:
    void cardAppend(Card* card);

public:
	GameView(const QString& name_red, const QString& name_blue, QWidget* parent = nullptr);

    void drawPlayerCards(const base::Player& player, QPointF start_point);
    void drawAvailablePositions(const base::Board& board);
    void drawExplosion(const base::ExplosionService& effect, uint16_t, QPointF start_point);
    void setDeckVisible(color::ColorType color, bool visible);
    void showWin(color::ColorType color);
    void setExplosionViewActive(const QPointF& p1, const QPointF& p2);

    void _initWebView();
private:
    void _drawSquareAt(QPointF pos);
    void _initLabels(const QString& name_red, const QString& name_blue);
    Card* _createCardAt(color::ColorType color, base::CombatCardType type, QPointF pos);
    
private:
    //QWebEngineView* webView;
    QVBoxLayout* layout;
    QLabel* infoLabel;
    QLabel* red_name_label;
    QLabel* blue_name_label;
    QLabel* won_label;

    QGraphicsScene* scene;
    ExplosionView* explosion;

    QList<Card*> red_deck;
    QList<Card*> blue_deck;
    QHash<QPointF, QGraphicsRectItem*> m_available_cells;
};

class ExplosionController : public QObject {
    Q_OBJECT

public:
    ExplosionController(QObject* parent, base::ExplosionService& service);
    void checkUpdate();

private:
    base::ExplosionService& model;
    ExplosionView* view;

public slots:
    
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

