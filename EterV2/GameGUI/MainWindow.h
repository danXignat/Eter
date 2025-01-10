#pragma once
#include <QDebug>
#include <QtWidgets/QMainWindow>

#include "qt_includes.h"
#include "settings.h"

#include "..\EterV2\GameModeFactory.h"
#include "..\EterV2\InputHandler.h"

class Card : public QObject, public QGraphicsItem {
    Q_OBJECT

public:
    explicit Card(color::ColorType, base::CombatCardType, const QString& imagePath, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    void moveCardBack();

    base::CombatCardType getType() const;
    color::ColorType getColor() const;
    bool isPlaced();
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

signals:
    void cardAppend(Card* card);
};

class RequestNameScene : public QWidget {
    Q_OBJECT

public:
    explicit RequestNameScene(QWidget* parent = nullptr);

signals:
    void nameEntered(const QString& bluePlayer, const QString& redPlayer);

private slots:
    void onNextClicked();

private:
    QLineEdit* playerBlueNameInput;
    QLineEdit* playerRedNameInput;
    QPushButton* nextButton;
};

class SelectModeScene : public QWidget {
    Q_OBJECT

public:
    explicit SelectModeScene(QWidget* parent = nullptr);

signals:
    void modeSelected(const std::string& mode);

private:
    QPushButton* trainingButton;
    QPushButton* mageDuelButton;
    QPushButton* elementalBattleButton;
};


class GameScene : public QWidget {
    Q_OBJECT

public:
    explicit GameScene(QWidget* parent = nullptr);

    void startGame(const std::string& mode, const std::string& playerBlueName, const std::string& playerRedName);
    void drawSquareAt(QPoint pos);
    void drawAvailablePositions();
    QGraphicsScene* getScene();

public slots:
    void onCardAppend(Card* card);

private:
    QVBoxLayout* layout;
    QLabel* infoLabel;

    QLabel* playerBlueNameLabel;
    QLabel* playerRedNameLabel;

    base::GameModePtr gamemode;
    /*const base::Player& player_one;
    const base::Player& player_two;
    const base::Board& game_board;*/
    Deck* playerBlue;
    Deck* playerRed;
    QGraphicsScene* scene;
    QGraphicsView* view;
};
class Deck : QWidget {

public:

    Deck(GameScene* gameControler, const base::Player& player, QPointF start_point);

    void drawPlayerCards();
    void createCardAt(color::ColorType color, base::CombatCardType type, QPointF pos);
    void showCards();
    void hideCards();
private:

    QPointF start_point;
    GameScene* gameControler;
    const base::Player& player;

};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onNameEntered(const QString& playerBlueName, const QString& playerRedName);
    void onModeSelected(const std::string& mode);

private:
    QStackedWidget* stackedWidget;
    RequestNameScene* requestNameScene;
    SelectModeScene* selectModeScene;
    GameScene* gameScene;

    QString playerBlueNameGlobal;
    QString playerRedNameGlobal;
    std::string selectedMode;
};

