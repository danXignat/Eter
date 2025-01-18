#pragma once
#include "qt_includes.h"
#include "game.h"
#include "settings.h"
#include "PauseMenuDialog.h"
#include "..\EterV2\GameModeFactory.h"
#include "..\EterV2\InputHandler.h"

class NextButton : public QPushButton {
    Q_OBJECT
public:

    explicit NextButton(QWidget* parent = nullptr);

protected:

    void paintEvent(QPaintEvent* event) override;

    void enterEvent(QEnterEvent* event) override;

    void leaveEvent(QEvent* event);

    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    bool event(QEvent* event);
private:
    QPixmap currentPixmap;
};

class RequestNameScene : public QWidget {
    Q_OBJECT

public:
    explicit RequestNameScene(QWidget* parent = nullptr);

signals:
    void nameEntered(const QString& bluePlayer, const QString& redPlayer);

private slots:
    void onNextClicked();
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    QLineEdit* playerBlueNameInput;
    QLineEdit* playerRedNameInput;
    NextButton nextButton;
    QPixmap background;
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
    explicit GameScene(const std::string& mode, const QString& playerBlueName, const QString& playerRedName, QWidget* parent = nullptr);

private:
    GameController controller;
};


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
private slots:
    void onNameEntered(const QString& playerBlueName, const QString& playerRedName);
    void onModeSelected(const std::string& mode);
    void onResumeGame();
    

private:
    QStackedWidget* stackedWidget;
    RequestNameScene* requestNameScene;
    SelectModeScene* selectModeScene;
    GameScene* gameScene = nullptr;
    PauseMenuScene* pauseMenuScene;
    QGraphicsRectItem* overlay = nullptr;
    QWidget* lastScene = nullptr;
    QString playerBlueNameGlobal;
    QString playerRedNameGlobal;
    std::string selectedMode;
};

