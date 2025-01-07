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

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QPixmap cardImage;
    QPointF lastMousePosition;
    color::ColorType color;
    base::CombatCardType type;

signals:
    void cardAppend(color::ColorType, base::CombatCardType, QPoint coord);
};

class RequestNameScene : public QWidget {
    Q_OBJECT

public:
    explicit RequestNameScene(QWidget* parent = nullptr);

signals:
    void nameEntered(const QString& name);

private slots:
    void onNextClicked();

private:
    QLineEdit* nameInput;
    QPushButton* nextButton;
};

class SelectModeScene : public QWidget {
    Q_OBJECT

public:
    explicit SelectModeScene(QWidget* parent = nullptr);

signals:
    void modeSelected(const std::string& mode);

private:
    QPushButton* easyButton;
    QPushButton* hardButton;
};

class GameScene : public QWidget {
    Q_OBJECT

public:
    explicit GameScene(QWidget* parent = nullptr);

    void startGame(const std::string& mode, const std::string& playerName);
    void createCardAt(color::ColorType color, base::CombatCardType type, QPointF pos);
    void drawPlayerCards(const base::Player& player, QPointF start_point);
    void drawSquareAt(QPoint pos);
    void drawAvailablePositions();

public slots:
    void onCardAppend(color::ColorType, base::CombatCardType, QPoint coord);

private:
    QVBoxLayout* layout;
    QLabel* infoLabel;

    base::GameModePtr gamemode;
    /*const base::Player& player_one;
    const base::Player& player_two;
    const base::Board& game_board;*/

    QGraphicsScene* scene;
    QGraphicsView* view;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onNameEntered(const QString& playerName);
    void onModeSelected(const std::string& mode);

private:
    QStackedWidget* stackedWidget;
    RequestNameScene* requestNameScene;
    SelectModeScene* selectModeScene;
    GameScene* gameScene;

    QString playerNameGlobal;
    std::string selectedMode;
};